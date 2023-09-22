#pragma once
#include <unordered_set>
#include <iostream>
#include "mandelbrot.h"
#define MAX_STEPS 20

void mb_single_step(std::complex<double>& z, std::complex<double> c) {
	z = (z * z) + c * 1.17;
}

std::complex<double> mb_multi_step(std::complex<double> c, int step_count) {
	auto z = std::complex<double>(0, 0);
	for (int i = 0; i < step_count; i++) {
		mb_single_step(z, c);
	}
	return z;
}

double is_value_on_set(std::complex<double> c) {
	//std::vector<std::complex<double>> previous;
	auto z = std::complex<double>(0, 0);
	int i;

	for (i = 0; i < MAX_STEPS; i++) {
		mb_single_step(z, c);

		if (norm(z) > 4) {
			break;
			
		}
		/*else if (std::count(previous.begin(), previous.end(), z) > 0) {
			return 1;
		}
		previous.push_back(z);*/
	}
	
	return ((double)i / MAX_STEPS);
	
}