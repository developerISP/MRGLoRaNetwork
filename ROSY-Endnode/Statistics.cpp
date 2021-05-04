#ifndef _STATISTICS_CPP_
#define _STATISTICS_CPP_
#include "Arduino.h"
#include "Statistics.h"

template <typename T>
static float ROSY::Statistics::calcAverage(T sum, int n)
{
	if (isnan(sum) || n <= 0) {
		return NAN;
	}
	return (float)sum / n;
}

template <typename T>
static float ROSY::Statistics::calcVarience(T sum, T sumPOW, int n)
{
	if (isnan(sum) || isnan(sumPOW) || n <= 0) {
		return NAN;
	}
	float avrg = Statistics::calcAverage<T>(sum, n);
	return (float)sumPOW / n - pow(avrg, 2);
}

template <typename T>
static float ROSY::Statistics::calcMSD(T sum, T sumPOW, int n)
{
	if (isnan(sum) || isnan(sumPOW) || n <= 0) {
		return NAN;
	}
	return sqrt(Statistics::calcVarience<T>(sum, sumPOW, n));
}

template <typename T>
static float ROSY::Statistics::calcVariationCoef(T sum, T sumPOW, int n)
{
	if (isnan(sum) || isnan(sumPOW) || n <= 0) {
		return NAN;
	}
	return Statistics::calcMSD<T>(sum, sumPOW, n) / Statistics::calcAverage<T>(sum, n);
}
#endif
