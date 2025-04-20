#include "search.h"
main(argc, argv)
int argc;
char** argv;
{
	enum action ac;
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: Provide starting path\n");
		_exit(1);
	}
	ac = copy_file;
	search(*(argv + 1), ac);
	_exit(0);
}
