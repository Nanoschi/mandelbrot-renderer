#pragma once
#include <complex>

void mb_single_step(std::complex<double>& z, std::complex<double> c);

double is_value_on_set(std::complex<double> c, int steps);