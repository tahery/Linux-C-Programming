#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
DIR * dir;
struct dirent * ptr;

int main(int argc,char *argv[])
{
	// declaration for different parameters
	void _a(char *direct);
	void _l(char *direct);
	void _i(char *direct);
	void _t(char *direct);

	// get the argc[]
	char para[20];
	char direct[100];
	memset(direct, 0, sizeof(direct));
	// copy the parameter like -l
	strcpy(para, argv[1]);
	// copy the path like ./
	strcpy(direct, argv[2]);

	// call the corresponding function according to parameters
	if (strcmp(para, "-a") == 0)
	{
		_a(direct);
	}
	if (strcmp(para, "-l") == 0)
	{
		_l(direct);
	}
	if (strcmp(para, "-i") == 0)
	{
		_i(direct);
	}
	if (strcmp(para, "-t") == 0)
	{
		_t(direct);
	}


}

void _a(char *direct)
{
	dir = opendir(direct);
	while((ptr = readdir(dir)) != NULL)
	{
		printf("%s\n", ptr->d_name);
	}
	closedir(dir);
}

void _l(char *direct)
{
	void mode_to_letter(int mode, char *str);
	struct stat fst;
	struct tm * mytime = (struct tm *) malloc(sizeof(struct tm));
	char str[12];
	dir = opendir(direct);
	readdir(dir);
	readdir(dir);
	while((ptr = readdir(dir)) != NULL)
	{
		stat(ptr->d_name, &fst);
		mode_to_letter(fst.st_mode, str);
		printf("%s", str);
		printf(" %d", fst.st_nlink);
		printf(" %s", getpwuid(fst.st_uid)->pw_name);
		printf(" %s", getgrgid(fst.st_gid)->gr_name);
		printf(" %ld", (long)fst.st_size);
		mytime = localtime(&fst.st_mtime);
		printf(" %d-%02d-%02d %02d:%02d", mytime->tm_year + 1900, mytime->tm_mon + 1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min);
		printf(" %s", ptr->d_name);
		printf("\n");
	}
}

void mode_to_letter(int mode, char *str)
{
	str[0] = '-';

	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';

	if(mode & S_IRUSR) str[1] = 'r';
	else str[1] = '-';
	if(mode & S_IWUSR) str[2] = 'w';
	else str[2] = '-';
	if(mode & S_IXUSR) str[3] = 'x';
	else str[3] = '-';

	if(mode & S_IRGRP) str[4] = 'r';
	else str[4] = '-';
	if(mode & S_IWGRP) str[5] = 'w';
	else str[5] = '-';
	if(mode & S_IXGRP) str[6] = 'x';
	else str[6] = '-';

	if(mode & S_IROTH) str[7] = 'r';
	else str[7] = '-';
	if(mode & S_IWOTH) str[8] = 'w';
	else str[8] = '-';
	if(mode & S_IXOTH) str[9] = 'x';
	else str[9] = '-';

	str[10] = '\0';
}

void _i(char *direct)
{
	dir = opendir(direct);
	readdir(dir);
	readdir(dir);	
	while((ptr = readdir(dir)) != NULL)
	{
		printf("%ld ", (long)ptr->d_ino);
		printf("%s\n", ptr->d_name);
	}
	closedir(dir);
}

void _t(char *direct)
{
	dir = opendir(direct);
	readdir(dir);
	readdir(dir);	
	while((ptr = readdir(dir)) != NULL)
	{
		printf("%ld ", (long)ptr->d_ino);
		printf("%s\n", ptr->d_name);
	}
	closedir(dir);
}