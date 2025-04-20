#include "search.h"

static char buf[buf_cap];
static char ppath[buf_cap];

static open_dir(dirp, name);
static close_dir(dirp);
static build_path(char* dest, ...);
static read_dir(name);

static open_dir(dirp, name)
DIR** dirp;
const char* name;
{
	*dirp = opendir(name);
	if (!*dirp)
	{
		perror("open_dir()\n");
		exit(1);
	}
}

static close_dir(dirp)
DIR* dirp;
{
	int res;
	errno = 0;
	res = closedir(dirp);
	if (res == -1)
	{
		perror("close_dir()\n");
		exit(2);
	}
}

static build_path(char* dest, ...)			/*incosistency with other code because of stdarg*/
{
	va_list vl;
	int i;
	const char* arg;
	size_t len;
	va_start(vl, dest);
	for (i = 0; (arg = va_arg(vl, const char*)); i++)
	{
		if (!i)
		{
			strcpy(dest, arg);
			continue;
		}
		strcat(dest, arg);
	}
	len = strlen(dest);
	*(dest + len) = 0;
	va_end(vl);
}

static read_dir(name)
const char* name;
{
	DIR* dirp;
	struct dirent* ent;
	struct stat statbuf;
	int res;
	size_t len;
	open_dir(&dirp, name);
	while ((ent = readdir(dirp)))
	{
		if (0 == strcmp(ent->d_name, ".") || 0 == strcmp(ent->d_name, "..") || 0 == strcmp(ent->d_name, ".git"))
			continue;
		switch (ent->d_type)
		{
			case DT_REG :
				build_path(ppath, name, BACK_SLASH, ent->d_name, NULL);
				printf("DEBUG_PRINT - %s\n", ppath);
				stat(ppath, &statbuf);
				*ppath = 0;
				break;
			case DT_DIR :
				read_dir(NULL);
				break;
		}
	}
	close_dir(dirp);
}

search(start)
const char* start;
{
	read_dir(start);
}
