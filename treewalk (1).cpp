/*
* Hai-Au Bui, File System Traversal
* CSD415, Professor Abbott
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>  

typedef int Myfunc ( const char *fpath, const struct stat *sb, int typeflag );
typedef char* permission( char* fpath );

static Myfunc myfunc;
// char *ultoa( unsigned long int value, char *buffer, int radix);

using namespace std;

static struct stat sbuff;
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main( int argc, char *argv[] ) {
	DIR *dir;
	int ret;

	if( argc != 2 ) {
		cerr << "usage: ftwalk pathname" << endl;
		exit( -1 );
	}

	

	ret = ftw( 
		argv[1],	// starting pathname
		myfunc,		// function to call each time
		50 			// number of open fd's
	);

	dir = opendir(argv[1]);
	if( dir == NULL ){
		cerr << "Directory not found - \'" << argv[1] << "\'" << endl;
		exit(-1);
	}

	printf("---Regular files: %ld\n", nreg);
	printf("---Directories  : %ld\n", ndir);
	printf("---Block special files: %ld\n", nblk);
	printf("---Character special files: %ld\n", nchr);
	printf("---FIFOs files  : %ld\n", nfifo);
	printf("---Sockets files: %ld\n", nsock);

	exit( ret );
}



static int myfunc( const char *pathname, const struct stat *statptr, int type ) {
	
	cout << pathname << endl;

	//file type 
	printf("\tFile type:         ");
	switch( statptr->st_mode & S_IFMT){
		case S_IFREG: printf("regular file\n");
		nreg++;
		break;

		case S_IFDIR: printf("directory\n");
		ndir++;
		break;

		case S_IFCHR: printf("character special\n");
		nchr++;
		break;

		case S_IFBLK: printf("block special\n");
		nblk++;
		break;	

		case S_IFIFO: printf("FIFOs/pipe\n");
		nfifo++;
		break;
		
		case S_IFLNK: printf("symbolic link\n");
		break;

		case S_IFSOCK: printf("socket\n");
		nsock++;
		break;

		default:	printf("unknown type");
		break;
	}

	//permissions
	printf("\tPermission:  	   ");
	//check permissio for owner
	if( statptr->st_mode & S_IRUSR) printf("%c",'r');
	if( statptr->st_mode & !S_IRUSR) printf("%c",'-');
	if( statptr->st_mode & S_IWUSR) printf("%c",'w');
	if( statptr->st_mode & !S_IWUSR) printf("%c",'-');
	if( statptr->st_mode & S_IXUSR) printf("%c",'x');
	else printf("%c",'-');

	//check permission for group
	if( statptr->st_mode & S_IRGRP) printf("%c",'r');
	if( statptr->st_mode & !S_IRGRP) printf("%c",'-');
	if( statptr->st_mode & S_IWGRP) printf("%c",'w');
	if( statptr->st_mode & !S_IWGRP) printf("%c",'-');
	if( statptr->st_mode & S_IXGRP) printf("%c",'x');
	else printf("%c",'-');

	//check permission for other
	if( statptr->st_mode & S_IROTH) printf("%c",'r');
	if( statptr->st_mode & !S_IROTH) printf("%c",'-');
	if( statptr->st_mode & S_IWOTH) printf("%c",'w');
	if( statptr->st_mode | !S_IWOTH) printf("%c",'-');
	if( statptr->st_mode & S_IXOTH) printf("%c",'x');
	else printf("%c",'-');

	//device number
	printf("\n\tDevice number:     %ld\n", (long)statptr->st_dev);

	//device number
	printf("\tDevice number (if special file): %ld\n", (long)statptr->st_rdev);

	//i-node number
	printf("\tI-node number:     %ld\n", (long)statptr->st_ino);

	//link count
	printf("\tLink count:        %ld\n", (long)statptr->st_nlink);

	//user Id of owner
	printf("\tOwner number:      UID = %ld    GID = %ld\n", (long)statptr->st_uid, (long)statptr->st_gid);

	//size in byte
	printf("\tSize in byte:      %lld bytes\n", (long long) statptr->st_size);

	//time of last access
	printf("\tTime of last access: %s", ctime(&statptr->st_atime));

	//time of last modification
	printf("\tLast modification:   %s", ctime(&statptr->st_mtime));

	//time of last file status change
	printf("\tLast status change:  %s", ctime(&statptr->st_ctime));

	//best I/O block size
	printf("\tI/O block size:    %ld bytes\n", (long)statptr->st_blksize);

	//number of blocks allocated
	printf("\tBlocks allocated:  %lld\n",(long long) statptr->st_blocks);



	return( 0 );
}

