#include <unistd.h>
main(argc, argv)
int argc;
char** argv;
{
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: Provide starting path\n");
		_exit(1);
	}
	search(*(argv + 1));
	_exit(0);
}
