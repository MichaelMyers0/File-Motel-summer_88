#include "search.h"

static char buf[buf_cap];
static char ppath[buf_cap];

static open_dir(dirp, name);
static close_dir(dirp);
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
				strcpy(ppath, name);
				strcat(ppath, BACK_SLASH);
				strcat(ppath, ent->d_name);
				len = strlen(ppath);
				*(ppath + len) = 0;
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
