#ifndef EUCLIDEAN_METRIC_HPP
#define EUCLIDEAN_METRIC_HPP

#include "euclidean.hpp"
#include<cmath>

class euclidean_metric{

	template<class T,unsigned D>
	static inline T norm2(const euclidean<T,D>& v){
		return v.reduce([](unsigned i,const T& f,const T& t){
			return f*f+t;
		},T{});
	}
	template<class T,unsigned D>
	static inline T norm(const euclidean<T,D>& v){
		return std::sqrt(norm2(v));
	}

	template<class T,unsigned D>
	static inline T distance2(const euclidean<T,D>& a,const euclidean<T,D>& b){
		return norm2(a-b);
	}

	template<class T,unsigned D>
	static inline T distance(const euclidean<T,D>& a,const euclidean<T,D>& b){
		return std::sqrt(distance2(a,b));
	}
}

#endif // POINT_HPP
