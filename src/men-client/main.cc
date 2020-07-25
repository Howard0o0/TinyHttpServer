#include "menclient.h"
#include <string>

void PrintUsage() {
	fprintf(stderr, "usage: men [keyword] \n");
}

int main(int argc, char** argv) {

	if (argc != 2) {
		PrintUsage();
		return -1;
	}

	MenClient   men_client("101.132.152.51", 10000);
	std::string result = men_client.Query(argv[ 1 ]);
	if (result.empty()) {
		fprintf(stderr,
			"can't find anything about %s, please make sure your "
			"keyword is a correct linux command line\n",
			argv[ 1 ]);
		return -1;
	}
	fprintf(stdout, "%s\n", result.c_str());

	return 0;
}