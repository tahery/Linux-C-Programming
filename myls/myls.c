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
	void _d(char *direct);
	void _t(char *direct);
	void _F(char *direct);
	void _R(char *direct);

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
	if (strcmp(para, "-d") == 0)
	{
		_d(direct);
	}
	if (strcmp(para, "-t") == 0)
	{
		_t(direct);
	}
	if (strcmp(para, "-F") == 0)
	{
		_F(direct);
	}
	if (strcmp(para, "-R") == 0)
	{
		_R(direct);
	}


}

void _a(char *direct)
{
	// -a list everyting include . and ..
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
	// define struct stat
	struct stat fst;
	struct tm * mytime = (struct tm *) malloc(sizeof(struct tm));
	char str[12];
	dir = opendir(direct);
	// skip . and ..
	readdir(dir);
	readdir(dir);
	while((ptr = readdir(dir)) != NULL)
	{
		stat(ptr->d_name, &fst);
		// permission information
		mode_to_letter(fst.st_mode, str);
		// file type and permission
		printf("%s", str);
		// file hard links
		printf(" %d", fst.st_nlink);
		// file's owner
		printf(" %s", getpwuid(fst.st_uid)->pw_name);
		// file's owner group
		printf(" %s", getgrgid(fst.st_gid)->gr_name);
		// file size
		printf(" %ld", (long)fst.st_size);
		// file time
		mytime = localtime(&fst.st_mtime);
		printf(" %d-%02d-%02d %02d:%02d", mytime->tm_year + 1900, mytime->tm_mon + 1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min);
		// file name
		printf(" %s", ptr->d_name);
		printf("\n");
	}
}

void mode_to_letter(int mode, char *str)
{
	str[0] = '-';

	// judge file type
	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';

	// judge permission for owner
	if(mode & S_IRUSR) str[1] = 'r';
	else str[1] = '-';
	if(mode & S_IWUSR) str[2] = 'w';
	else str[2] = '-';
	if(mode & S_IXUSR) str[3] = 'x';
	else str[3] = '-';

	// judge permission for owner group
	if(mode & S_IRGRP) str[4] = 'r';
	else str[4] = '-';
	if(mode & S_IWGRP) str[5] = 'w';
	else str[5] = '-';
	if(mode & S_IXGRP) str[6] = 'x';
	else str[6] = '-';

	// judge permission for others
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
		// get inode id
		printf("%ld ", (long)ptr->d_ino);
		printf("%s\n", ptr->d_name);
	}
	closedir(dir);
}

void _d(char *direct)
{
	printf("%s\n", direct);
}

void _t(char *direct)
{
	struct stat fst;
	struct tm * mytime = (struct tm *) malloc(sizeof(struct tm));
	// imitation for vector
	struct fileinfo{
		char name[255];
		int year;
		int month;
		int day;
		int seconds;
	};
	struct fileinfo fileVector[100];
	dir = opendir(direct);
	readdir(dir);
	readdir(dir);
	int count = 0;
	// put all the info into vector to sort
	while((ptr = readdir(dir)) != NULL)
	{
		stat(ptr->d_name, &fst);
		mytime = localtime(&fst.st_mtime);
		fileVector[count].year = mytime->tm_year + 1900;
		fileVector[count].month = mytime->tm_mon + 1;
		fileVector[count].day = mytime->tm_mday;
		fileVector[count].seconds = mytime->tm_hour * 3600 + mytime->tm_min * 60;
		strcpy(fileVector[count].name, ptr->d_name);
		count = count + 1;
	}
	// start sorting
	int i, j;
	for (i = 0; i < count; ++i)
	{
		for (j = i+1; j < count; ++j)
		{
			// if i is newer than j
			if (fileVector[j].year>fileVector[i].year||(fileVector[j].year==fileVector[i].year&&fileVector[j].month>fileVector[i].month)||(fileVector[j].year==fileVector[i].year&&fileVector[j].month==fileVector[i].month&&fileVector[j].day>fileVector[i].day)||(fileVector[j].year==fileVector[i].year&&fileVector[j].month==fileVector[i].month&&fileVector[j].day==fileVector[i].day&&fileVector[j].seconds>fileVector[i].seconds))
			{
				// Let's do swapping
				fileVector[99].year = fileVector[i].year;
				fileVector[i].year = fileVector[j].year;
				fileVector[j].year = fileVector[99].year;
				fileVector[99].month = fileVector[i].month;
				fileVector[i].month = fileVector[j].month;
				fileVector[j].month = fileVector[99].month;
				fileVector[99].day = fileVector[i].day;
				fileVector[i].day = fileVector[j].day;
				fileVector[j].day = fileVector[99].day;
				fileVector[99].seconds = fileVector[i].seconds;
				fileVector[i].seconds = fileVector[j].seconds;
				fileVector[j].seconds = fileVector[99].seconds;
				strcpy(fileVector[99].name, fileVector[i].name);
				strcpy(fileVector[i].name, fileVector[j].name);
				strcpy(fileVector[j].name, fileVector[99].name);
			}
		}
	}
	for (i = 0; i < count; ++i)
	{
		// print them out
		printf("%s\t", fileVector[i].name);
	}
	printf("\n");
}

void _F(char *direct)
{
	dir = opendir(direct);
	readdir(dir);
	readdir(dir);
	int flag = 0;
	while((ptr = readdir(dir)) != NULL)
	{
		struct stat fst;
		flag = 0;
		stat(ptr->d_name, &fst);
		// if directory
		if(ptr->d_type & DT_DIR) 
		{
			flag = 1;
			printf("%s/ ",ptr->d_name);
		}
		// if symbolic link
		if(S_ISLNK(fst.st_mode)) 
		{
			flag = 1;
			printf("%s@ ",ptr->d_name);
		}
		// if socket
		if(S_ISSOCK(fst.st_mode)) 
		{
			flag = 1;
			printf("%s= ",ptr->d_name);
		}
		// if executable
		if(fst.st_mode & S_IXUSR) 
		{
			flag = 1;
			printf("%s* ",ptr->d_name);
		}
		switch(fst.st_mode)
		{
			// if FIFO
			case S_IFIFO: printf("%s| ",ptr->d_name);flag=1;break;
			// if whiteout
			case S_IFWHT: printf("%s%% ",ptr->d_name);flag=1;break;
		}
		// if ordinary file
		if(S_ISREG(fst.st_mode) && flag == 0) 
		{
			printf("%s ",ptr->d_name);
		}
	}
	printf("\n");
}

void _R(char *direct)
{
	dir = opendir(direct);

	while((ptr = readdir(dir)) != NULL)
	{
		// skip . and ..
		if(!strcmp(ptr->d_name,".") || !strcmp(ptr->d_name,".."))
			continue;
		// if directory
		if (ptr->d_type & DT_DIR)
		{
			printf("%s: \n", ptr->d_name);
			// do recursively
			_R(ptr->d_name);
		}
		else
			// print the name
			printf("%s ", ptr->d_name);
	}
}