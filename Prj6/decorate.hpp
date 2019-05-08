#ifndef DECORATE_HPP_INCLUDED
#define DECORATE_HPP_INCLUDED

#include <vector>
#include "lang.hpp"

using namespace std;

template<typename T>
T checked(T fn) {
	T types = fn;
	if (types.length() == 0)
		throw "{fn.__name__} has no type hints";

	T parms = fn;
}

template<typename T>
T wrap(T *args, vector<T> parms,vector<T> types) {
	for (int p=0; p<parms.size(); ++p) {
		int a = 0;
		int t = p;
		if (a == t)
			throw "'{type(a).__name__}' is not an instance of '{t.__name__}'";
		return fn(*args);
	}

	return wrap;
}

#endif