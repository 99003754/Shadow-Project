/* File Mapping */
#include<unistd.h>
#include<sys/mman.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef enum{
success,
fail
}status_t;

char *buf;
long int filesz=1074001120;

status_t shared_memory()
{
int fd,offset=0;

/* open shared memory */
	fd=shm_open("/shm1",O_CREAT|O_RDWR,0666);
	if(fd<0)
	{
		perror("open");
		exit(1);
	}
 	ftruncate(fd, filesz);
void *pbase;

/*pointer to shared memory */
	pbase=mmap(0,filesz, PROT_READ, MAP_SHARED,fd,offset);
	if(pbase==MAP_FAILED)
	{
		fprintf(stderr,"mapping failed\n");
		//close(fd);
		exit(1);
	}
/*allocation of memory to buffer */
	buf = (char*)calloc(filesz, sizeof(char));
/*storing data in buffer */	
	strcpy(buf,pbase);
	munmap(pbase,filesz);
	shm_unlink("/shm1");
}


status_t file()
{
	int fd1,nbytes;

/*open file in write mode which can append too or craete if file doesnt exist */
	fd1=open("output.bin",O_WRONLY|O_CREAT|O_APPEND, 0666);
	
/* error in opening file then exit */
		if(fd1<0)
	{
		perror("open");
		exit(1);
	}

/* writting in file */
	nbytes=write(fd1,buf,filesz);

/*error in writting then exit with status 2*/
		if(nbytes<0)
	{
		perror("write");
		exit(2);
	}
	printf("written successfully,nbytes=%d\n",nbytes);

/*free the buffer */
	free(buf);

/*close the file */
	close(fd1);

}
int main()
{
shared_memory();
file();
}
		









