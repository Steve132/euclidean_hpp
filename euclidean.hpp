#ifndef EUCLIDEAN_HPP
#define EUCLIDEAN_HPP

#include<array>
#include<cstdint>
#include<cstdlib>
#include<iostream>

namespace
{
	template<typename TupleOut,typename Func, std::size_t... I>
	static inline TupleOut tuple_index_apply_internal(Func f, std::index_sequence<I...>)
	{
		return TupleOut{f(I)...};
	}
	template<typename TupleOut,unsigned D,typename Func,typename Indices = std::make_index_sequence<D>>
	static inline TupleOut tuple_index_apply(Func f)
	{
		return tuple_index_apply_internal<TupleOut>(f,Indices{});
	}

	template<typename Func, std::size_t... I>
	static inline void index_apply_internal(Func f,std::index_sequence<I...>)
	{
		(...,f(I));
	}
	template<unsigned D,typename Func,typename Indices = std::make_index_sequence<D>>
	static inline void index_apply(Func f)
	{
		index_apply_internal(f,Indices{});
	}
	template<unsigned D,typename Func,typename T>
	static inline T reduce_index_apply_internal(Func f,T init,std::index_sequence<>)
	{
		return init;
	}
	template<unsigned D,typename Func,typename T,size_t cur,std::size_t... I>
	static inline T reduce_index_apply_internal(Func f,T init,std::index_sequence<cur,I...>)
	{
		return f(D-cur-1,reduce_index_apply_internal<D>(f,init,std::index_sequence<I...>{}));
	}
	template<unsigned D,typename Func,typename T,typename Indices = std::make_index_sequence<D>>
	static inline T reduce_index_apply(Func f,T init)
	{
		return reduce_index_apply_internal<D>(f,init,Indices{});
	}
}

template<class T,unsigned int D>
struct euclidean: public std::array<T,D>
{// Convert array into a tuple
private:

public:
	using std::array<T,D>::array;
	using std::array<T,D>::operator[];

	template<typename Func,typename IT,typename Indices = std::make_index_sequence<D>>
	IT reduce(Func f,IT init) const
	{
		return ::reduce_index_apply<D>(
			[this,&f](size_t i,IT t){
				return f(i,this->operator[](i),t);
			},init);
	}
	template<typename Func,typename Indices = std::make_index_sequence<D>>
	auto transform(Func f) const
		-> euclidean<decltype(f(0,T{})),D>
	{
		return ::tuple_index_apply<
			euclidean<decltype(f(0,T{})),D>,D
			>([this,&f](size_t i){
				return f(i,this->operator[](i));
			});
	}
	template<typename Func,typename Indices = std::make_index_sequence<D>>
	void for_each(Func f) const
	{
		::index_apply<D>([this,&f](size_t i){
			f(i,this->operator[](i));
		});
	}
	template<typename Func,typename Indices = std::make_index_sequence<D>>
	void for_each(Func f)
	{
		::index_apply<D>([this,&f](size_t i){
			f(i,this->operator[](i));
		});
	}

	template<typename... Args>
	euclidean(Args&& ...args):
		std::array<T,D>{std::forward<Args>(args)...}
	{}

	template<class OT>
	euclidean& operator+=(const euclidean<OT,D>& o){
		for_each([o](size_t i,T& v){
			v+=o[i];
		});
		return *this;
	}
	template<class OT>
	euclidean& operator-=(const euclidean<OT,D>& o){
		for_each([o](size_t i,T& v){
			v-=o[i];
		});
		return *this;
	}
	template<class OT>
	euclidean& operator*=(OT o){
		for_each([o](size_t i,T& v){
			v*=o;
		});
		return *this;
	}
	template<class OT>
	euclidean& operator/=(OT o){
		for_each([o](size_t i,T& v){
			v/=o;
		});
		return *this;
	}

	template<class OT>
	euclidean<decltype(T{}+OT{}),D>
	operator+(const euclidean<OT,D>& o){
		return transform([o](size_t i,T v){
			return v+o[i];
		});
	}
	template<class OT>
	euclidean<decltype(T{}-OT{}),D>
	operator-(const euclidean<OT,D>& o){
		return transform([o](size_t i,T v){
			return v-o[i];
		});
	}
	template<class OT>
	euclidean<decltype(T{}*OT{}),D>
	operator*(OT o){
		return transform([o](size_t i,T v){
			return v*o;
		});
	}
	template<class OT>
	euclidean<decltype(T{}/OT{}),D>
	operator/(OT o){
		return transform([o](size_t i,T v){
			return v/o;
		});
	}
};

template<class T,unsigned int D>
std::ostream& operator<<(std::ostream& out,const euclidean<T,D>& v)
{
	out << "{";
	v.for_each([&out](size_t i,float f){
		out << f;
		if(i != (D-1)){
			out << ",";
		}
	});
	return out;
}

#endif
