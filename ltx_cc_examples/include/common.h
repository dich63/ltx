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

	template <class C, int N>
	struct bN_t {
		C r[N];
		inline static bN_t* ptr(void* p) {
			return (bN_t *)p;
		}
	};

	template <int N>
	struct pN_t {
		
		template <class C>
		inline static bN_t<C,N>* ptr(C* p) {
			bN_t<C, N> tmp;
			return (bN_t<C, N>*)p;
		}
		template <class C>
		inline static bN_t<C, N>& ref(C* p) {
			return *ptr(p);
		}

	};


