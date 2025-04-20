#include "search.h"

static char buf[buf_cap];
static char ppath[buf_cap];
static char copy_buf[BUFSIZ];
static char f_path[buf_cap / 2];
static char f_path1[buf_cap / 2];

static open_dir(dirp, name);
static close_dir(dirp);
static build_path(char* dest, ...);
static copy(dst, src);
static check_file(name, suffix);
static read_dir(name, ac);
static open_file(path, fd, flags, mode, msg);
static close_file(fd, msg);
static get_inode_info(name, statbuf);
static make_a_back_up_copy(start, name, back_up_path);

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

static open_file(path, fd, flags, mode, msg)
const char* path;
int* fd;
int flags;
int mode;
const char* msg;
{
        errno = 0;
        *fd = open(path, flags, mode);
        if (*fd == -1)
        {
                perror(msg);
                exit(3);
        }
}

static close_file(fd, msg)
int fd;
const char* msg;
{
	int res;
        errno = 0;
        res = close(fd);
        if (res == -1)
        {
                perror(msg);
                exit(7);
        }	
}

static copy(dst, src)
const char* dst;
const char* src;
{
	int dst_fd, src_fd;
	int res;
	ssize_t cnt;

	open_file(dst, &dst_fd, O_CREAT | O_TRUNC | O_RDWR, 0666, "copy()->open(src)\n");
	open_file(src, &src_fd, O_RDWR, 0, "copy()->open(src)\n");

	errno = 0;
	cnt = read(src_fd, copy_buf, BUFSIZ);
	if (cnt == -1)
	{
		perror("copy()->read(src_fd)\n");
		exit(5);
	}
	*(copy_buf + cnt) = 0;
	write(dst_fd, copy_buf, cnt);
	*copy_buf = 0;

	close_file(dst_fd, "copy()->>close(dst_fd)\n");
	close_file(src_fd, "copy()->>close(src_fd)\n");
}

static check_file(name, suffix)
const char* name;
const char* suffix;
{
	size_t len;
	len = strlen(name);
	return *(name + len - 1) == *(suffix + 1) ? 1 : 0;	
}

static get_inode_info(name, statbuf)
const char* name;
struct stat* statbuf;
{
	int res;
	errno = 0;
	res = stat(name, statbuf);
	if (res == -1)
	{
		perror("get_inode_info()\n");
		exit(6);
	}
}

static make_a_back_up_copy(start, name, back_up_path)
const char* start;	
const char* name;
const char* back_up_path;
{
	DIR* dirp;
	struct stat statbuf1, statbuf2;
	int res;
	size_t len, len2;
	struct dirent* ent;
	char* s;
	open_dir(&dirp, start);
	len = strlen(name);
	get_inode_info(name, &statbuf1);
	while ((ent = readdir(dirp)))
	{
		if (0 == strcmp(ent->d_name, ".") || 0 == strcmp(ent->d_name, ".."))
			continue;
		if (0 == strncmp(name + 2, ent->d_name, len))
		{
			get_inode_info(back_up_path, &statbuf2);
			if (statbuf1.st_size != statbuf2.st_size)
			{
				printf("DEBUG_WE_HERE\n");
				build_path(f_path1, back_up_path, "_", "v_2", NULL);
				copy(f_path1, name);
			}
		}
	}
	close_dir(dirp);
}

static read_dir(name, ac)
const char* name;
enum action ac;
{
	DIR* dirp;
	struct dirent* ent;
	struct stat statbuf;
	int res;
	size_t len;
	open_dir(&dirp, name);
	while ((ent = readdir(dirp)))
	{
		if (0 == strcmp(ent->d_name, ".") || 0 == strcmp(ent->d_name, "..") || 0 == strcmp(ent->d_name, ".git") || 0 == strcmp(ent->d_name, "main"))
			continue;
		switch (ent->d_type)
		{
			case DT_REG :
				stat(ppath, &statbuf);
				build_path(ppath, name, BACK_SLASH, ent->d_name, NULL);
				printf("DEBUG_PRINT - %s\n", ppath);
				if (ac == copy_file && !check_file(ent->d_name, ".o"))
				{
					build_path(f_path, PATH, ppath + 1);
					printf("DEBUG_PRINT - %s\n", f_path);
					copy(f_path, ppath);
				}
				if (ac == backup_copy)
				{
					build_path(f_path, PATH, ppath + 1, NULL);
					printf("AC==BACKUP_COPY -%s\n", f_path);
					make_a_back_up_copy(PATH, ppath, f_path);
					*f_path = 0;
				}
				*ppath = 0;
				break;
			case DT_DIR :
				/*read_dir(NULL);*/
				break;
		}
	}
	close_dir(dirp);
}

search(start, ac)
const char* start;
enum action ac;
{
	read_dir(start, ac);
}
