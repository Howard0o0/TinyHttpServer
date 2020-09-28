#include "regextool.h"

std::vector< std::string > RegexTool::RegexMatch(const std::string& str, const std::string& pattern,
						 bool ignore_case, int case_cnt) {
	std::regex regex_pattern;
	if (ignore_case)
		regex_pattern = std::regex(pattern.c_str(), std::regex_constants::icase);
	else
		regex_pattern = pattern;

	std::smatch		    results;
	std::string::const_iterator iter_begin = str.begin();
	std::string::const_iterator iter_end   = str.end();

	if (!std::regex_search(iter_begin, iter_end, results, regex_pattern))
		return {};

	std::vector< std::string > match_results;
	for (int i = 0; i < case_cnt; ++i)
		match_results.push_back(results[ i + 1 ]);
	return match_results;
}