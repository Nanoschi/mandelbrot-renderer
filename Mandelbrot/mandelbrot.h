#pragma once
#include <complex>

void mb_single_step(std::complex<double>& z, std::complex<double> c);

std::complex<double> mb_multi_step(std::complex<double> c, int step_count);

double is_value_on_set(std::complex<double> c);