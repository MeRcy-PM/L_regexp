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
		start_pos = s;
		char *old_start = s;
		while (*s != '\0') {
			match_status (s, s, 0);
			if (old_start != start_pos) {
				s += (result.back ()).size ();
				old_start = start_pos;
			}
			else
				s++;
		}
	}
private:
	bool is_end_status (int stat) {return stree->last_op->at (stat);}
	void record_match_string (char *start, char *s) {
		string tmp;
		if (start == start_pos) {
			if (result.size () == 0) {
				tmp.assign (start, 0, s - start);
				result.push_back (tmp);
			}
			tmp = result.back ();
			tmp.append (start, tmp.size (), (unsigned long)(s - start) - tmp.size ());
		}
		else {
			tmp.assign (start, 0, s - start);
			start_pos = start;
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
};

#endif
