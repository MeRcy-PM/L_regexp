#ifndef __MATCHER_H__
#define __MATCHER_H__
#include "base.h"
#include "dfa.hpp"
#include <string>
using std::string;

class matcher {
public:
	matcher (stree_p syntax_tree, struct vertex* gstat, unsigned nstat)
				: stat (gstat),
				  stree (syntax_tree),
				  tstat (nstat) {};
	~matcher () {};
	void print_result () {
		if (result.empty ())
			cout << "No match result." << endl;
		else {
			for (unsigned i = 0; i < result.size (); i++)
				cout << "The " << i + 1 << "th match string: " << result.at (i) << endl;
		}
	}
	void match_string (char *s) {
		while (*s != '\0') {
			matched = false;
			match_status (s, s, 0);
			if (matched)
				s += (result.back ()).size ();
			else
				s++;
		}
	}
	bool match_test (vector<string> exp)
	{
		if (exp.size () != result.size ())
			return false;

		for (unsigned int i = 0; i < result.size (); i++) {
			if (result.at (i) != exp.at (i))
				return false;
		}
		return true;
	}
private:
	bool is_end_status (int stat) {return stree->last_op->at (stat);}
	void record_match_string (char *start, char *s) {
		string tmp;
		matched = true;
		if (start == start_pos) {
			if (result.size () == 0) {
				tmp.assign (start, 0, s - start);
				result.push_back (tmp);
				return;
			}
			tmp.assign (start, 0, s - start);
			result.pop_back ();
			result.push_back (tmp);
		}
		else {
			start_pos = start;
			tmp.assign (start, 0, s - start);
			result.push_back (tmp);
		}
	}
	void match_status (char *start, char *s, int nstat) {
		if (is_end_status (nstat)) {
			record_match_string (start, s);
		}

		if (*s == '\0')
			return;

		for (unsigned i = 0; i < tstat; i++)
			if (stat[nstat].edge->at (i))
				if (stat[i].id == *s)
					match_status (start, s + 1, i);
	}
	vector<string> result;
	struct vertex* stat;
	char *start_pos;
	stree_p stree;
	unsigned tstat;
	bool matched;
};

#endif
