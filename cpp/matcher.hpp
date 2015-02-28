#ifndef __MATCHER_H__
#define __MATCHER_H__
#include "base.h"
#include "dfa.hpp"
#include <string>
using std::string;

class matcher {
public:
	matcher () {};
	~matcher () {};
	void print_result () {
		if (result.empty ())
			cout << "No match result." << endl;
		else {
			for (int i = 0; i < result.size (); i++)
				cout << "The " << i + 1 << "match string: " << result.at (i) << endl;
		}
	}
	void match_string (struct vertex* stat, char *s) {
	
	}
private:
	vector<string> result;
};

#endif
