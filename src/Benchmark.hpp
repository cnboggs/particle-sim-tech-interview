#pragma once

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <string>
#include <vector>

// Allow for different time units to be used
template <typename TimeUnit = std::chrono::microseconds>
class Benchmark {
public:
	explicit Benchmark(std::string aName, const uint32_t aNumRuns = 1U)
		: mName{ std::move(aName) }
		, mNumRuns{ aNumRuns }
	{
		mRuns.reserve(aNumRuns);
	}

	template <typename Func>
	void Run(Func&& aFunc) {
		std::cout << std::format("Running Benchmark: {}\nNumber of runs = {}", mName, mNumRuns) << std::endl;

		for (auto i{ 0U }; i < mNumRuns; ++i) {
			const auto start{ std::chrono::steady_clock::now() };

			aFunc();

			const auto end{ std::chrono::steady_clock::now() };

			const auto duration{ std::chrono::duration_cast<TimeUnit>(end - start) };
			std::cout << std::format("Run {}: {}", i + 1, duration) << std::endl;

			mRuns.push_back(duration);
		}

		PrintResults();
	}

private:
	void PrintResults() const {
		TimeUnit totalDuration{};
		for (const auto& duration : mRuns) {
			totalDuration += duration;
		}

		const auto averageDuration{ std::chrono::duration_cast<TimeUnit>(totalDuration / mRuns.size()) };
        const auto [minIt, maxIt]{ std::minmax_element(mRuns.begin(), mRuns.end()) };
		std::cout << std::format("\nTotal: {}\nAverage: {}\nMin: {}\nMax: {}", totalDuration, averageDuration, *minIt, *maxIt) << std::endl;
	}

	std::string           mName;
	uint32_t              mNumRuns;
	std::vector<TimeUnit> mRuns;
};
