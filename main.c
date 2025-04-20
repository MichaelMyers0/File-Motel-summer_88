#include "search.h"
main(argc, argv)
int argc;
char** argv;
{
	enum action ac;
	if (argc < 3)
	{
		fprintf(stderr, "ERROR: One of the argumets are missing\n");
		_exit(1);
	}
	ac = (*argv + 2)[0] - '0';
	search(*(argv + 1), ac);
	_exit(0);
}
