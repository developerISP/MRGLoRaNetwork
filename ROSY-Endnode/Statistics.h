#ifndef _STATISTICS_H_
#define _STATISTICS_H_

namespace ROSY
{
	class Statistics
	{
	public:
		template <typename T>
		static float ROSY::Statistics::calcAverage(T sum, int n);
	public:
		template <typename T>
		static float ROSY::Statistics::calcVarience(T sum, T sumPOW, int n);
	public:
		template <typename T>
		static float ROSY::Statistics::calcMSD(T sum, T sumPOW, int n);
	public:
		template <typename T>
		static float ROSY::Statistics::calcVariationCoef(T sum, T sumPOW, int n);
	};
}
#include "Statistics.cpp"
#endif