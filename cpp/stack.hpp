#ifndef __STACK_H__
#define __STACK_H__
#include "base.h"

template <class T> class stack {
public:
	stack () :element (vector<T>()){}
	~stack () {}
	void print () {
		if (element.empty ()) {
			cout << "Empty stack." <<endl;
			return;
		}

		for (typename vector<T>::iterator iter = element.begin ();
			 iter != element.end (); ++iter) {
			cout << *iter << endl;
		}
	}
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

template <class T> class ops : private stack<T> {
public:
	ops () {}
	~ops () {}
	void print_stack () {
		this->print ();
	}
	/* Push fail means it's needed adjust.  */
	bool push (T ele) {
		if (is_need_adjust ())
			return false;

		this->push_one_element (ele);
		return true;
	}
	T pop () {
		return this->pop_one_elment ();
	}
	bool is_need_adjust () {
		return false;
	}
};

template <class T> class syms : private stack<T> {
public:
	syms () {}
	~syms () {}
	void print_stack () {
		this->print ();
	}
	void push (T ele) {
		this->push_one_element (ele);
	}
	T pop () {
		return this->pop_one_element ();
	}
};
#endif
