#ifndef __STACK_H__
#define __STACK_H__
#include "base.h"

template <typename T> class istack {
public:
	virtual void push_one_element (T ele) = 0;
	virtual T pop_one_element () = 0;
};

template <class T> class ops : private istack<T>{
public:
	ops () :element(vector<T>()){}
	~ops () {}
	void print_ops () {
		if (element.empty ()) {
			cout << "Empty stack." <<endl;
			return;
		}

		for (typename vector<T>::iterator iter = element.begin ();
			 iter != element.end (); ++iter) {
			cout << *iter << endl;
		}
	}
	void push (T ele) {

	}
	T pop () {
		return this->pop_one_element ();
	}
private:
	void push_one_element (T ele) {
		element.push_back (ele);
	}
	T pop_one_element () {
		T temp = element.back ();
		element.pop_back ();
		return temp;
	}
	vector<T> element;
};

#endif
