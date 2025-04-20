#ifndef SEARCH_H
#define SEARCH_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
enum {buf_cap = 1024};
enum action {copy_file, backup_copy};
enum {execute_bit = 0xA00};
#define BACK_SLASH "/"
#define PATH "./backup_storage"
search(start, ac);
#endif
