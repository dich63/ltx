// ltx_test1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <chrono>

template <int _V = 0>
struct stopwatch_t {
	std::chrono::high_resolution_clock::time_point t;

	stopwatch_t() {
		if (!_V) start();
	};

	inline void start() {
		t = std::chrono::high_resolution_clock::now();
	}
	inline double sec() {

		auto tn = std::chrono::high_resolution_clock::now();
		auto time_span = tn - t;
		std::chrono::duration<double> dt = time_span;
		return dt.count();
	}

};

// TODO: Reference additional headers your program requires here.
