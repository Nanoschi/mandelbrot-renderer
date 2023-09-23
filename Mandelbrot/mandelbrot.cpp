#pragma once
#include <unordered_set>
#include <iostream>
#include "mandelbrot.h"

void mb_single_step(std::complex<double>& z, std::complex<double> c) {
	z = (z * z) + c * 1.17;
}

double is_value_on_set(std::complex<double> c, int steps) {
	auto z = std::complex<double>(0, 0);
	int i;

	for (i = 0; i < steps; i++) {
		mb_single_step(z, c);

		if (norm(z) > 4) {
			break;
			
		}	
	}	
	return ((double)i / steps);
	
}