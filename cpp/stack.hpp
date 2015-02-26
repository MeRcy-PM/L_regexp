#ifndef __STACK_H__
#define __STACK_H__
#include "base.h"

template <class T> class stack {
public:
	stack () :element (vector<T>()){}
	virtual ~stack () {};
	T curr () {return element.back ();}
	bool is_empty () {return element.empty ();}
	void inline print_stack () {
		if (element.empty ()) {
			cout << "Empty stack." <<endl;
			return;
		}
 
		for (typename vector<T>::iterator iter = element.begin ();
			 iter != element.end (); ++iter) {
			cout << *iter << endl;
		}
	}
	virtual bool is_need_adjust (T ele) = 0;
	void push (T ele) {
		element.push_back (ele);
	}
	T pop () {
		if (element.empty ())
			assert (0);
		T temp = element.back ();
		element.pop_back ();
		return temp;
	}
private:
	vector<T> element;
};

/* Declare operator stack.  */
template <class T> class ops : public stack<T> {
public:
	ops () {}
	~ops () {}
	/* Push fail means it's needed adjust.  */
	bool is_need_adjust (T ele) {
		if (this->is_empty ())
			return false;

		if (ele >= (this->curr ()))
			return true;
	
		return false;
	}
};

/* Declare symbol stack.  */
template <class T> class syms : public stack<T> {
public:
	syms () {}
	~syms () {}
	bool is_need_adjust (T ele) {return false;}
};
#endif
