#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/shm.h>
#include<fcntl.h>



char    *buffer;
long    numbytes;

typedef enum{
success,
fail,
fail_fileopen,
memory_error
}enum_t;


enum_t opening_file()
{
FILE    *infile;

/* open an existing file for reading */
infile = fopen("input.bin", "r");
 
/* quit if the file does not exist */
if(infile == NULL)
    return fail_fileopen;
 
/* Get the number of bytes */
fseek(infile, 0L, SEEK_END);
numbytes = ftell(infile);
printf("%ld",numbytes);

/* reset the file position indicator to the beginning of the file */
fseek(infile, 0L, SEEK_SET);	

/* grab sufficient memory for the buffer to hold the text */
buffer = (char*)calloc(numbytes, sizeof(char));	


/* memory error */
if(buffer == NULL)
    return memory_error;
 
/* copy all the text into the buffer */
fread(buffer, sizeof(char), numbytes, infile);
fclose(infile);
return success;

}


enum_t shared_memory()
{
int 	fd,offset=0;
/* open shared memory */
fd=shm_open("/shm1",O_CREAT|O_RDWR,0666);
/*exit if file is not correct */
	if(fd<0)
	{
		perror("open");
		exit(1);
	}
ftruncate(fd,numbytes); 
void *pbase;
/*pointer to shared memory */
pbase=mmap(0,numbytes, PROT_WRITE, MAP_SHARED,fd,offset);
	if(pbase==MAP_FAILED)
	{
		fprintf(stderr,"mapping failed\n");
		//close(fd);
		exit(1);
	}
/* copy data to buffer */
	strcpy(pbase,buffer);
	munmap(pbase,numbytes);
	free(buffer);
return success;
}

int main()
{
int status;
status=opening_file();
status=shared_memory();
}
