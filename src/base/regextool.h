#ifndef SHADOWHTTP_REGEXTOOL_H
#define SHADOWHTTP_REGEXTOOL_H

#include <regex>
#include <string>
#include <vector>

class RegexTool {
    public:
	static std::vector< std::string > RegexMatch(const std::string& str,
						     const std::string& pattern, bool ignore_case,
						     int case_cnt);
};

#endif