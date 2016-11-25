#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
//#include <sys/syslimits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "support.h"


int finalLru=0;
struct Node **cache;


struct Node
{
  char *fileContents;
  char *filename;
  int lruCount;
};

int numCacheElements()
{
  int count;
  int elementCounter=0;
  for(count=0;count<finalLru;count++)
    {
      if(cache[count] != NULL)
	       elementCounter++;
    }
  
  return elementCounter;
}

void removeLru()
{
  
  int count;
  int maxIndex=0;
  for(count=0;count<finalLru;count++)
    {
      if(cache[count]->lruCount > cache[maxIndex]->lruCount)
        maxIndex = count;
   }
   cache[maxIndex]=NULL;
}

void incrementer()
{

  int  i;
  for(i=0;i<finalLru;i++)
    {
      if(cache[i]!=NULL)
        {
          int temp = cache[i]->lruCount;
	         cache[i]->lruCount = temp + 1;
        }
    }
}

void addNode(char *fc, char *fn)
{
 
  struct Node *n=malloc(sizeof(struct Node));
  n->fileContents=malloc(strlen(fc));
  n->filename = malloc(strlen(fn));
  strcpy(n->fileContents,fc);
  strcpy(n->filename,fn);
  n->lruCount=1;
   

  incrementer();

  //if we cache contains filename already, overwrite it
  int cnt;
  for(cnt=0;cnt<finalLru;cnt++)
    {
     if( cache[cnt]!=NULL)  
	   {
	     if(strcmp(cache[cnt]->filename,fn)==0)
	      {
	      cache[cnt]=NULL;
	      cache[cnt]=n;
	      return;
	     }
	    }
    }

  //make sure cache isnt filled
  if(numCacheElements() == finalLru)
    removeLru();

  int count;
  for(count =0; count<finalLru;count++)
    {   
      if(cache[count]==NULL)
	     {
	       cache[count] = n;
	       return;
	   }
    }
}

char* searcher(char* fn)
{
  int count;
  for(count=0;count<finalLru;count++)
    {
      if(cache[count]!=NULL)
	{
	  if(strcmp(cache[count]->filename,fn)==0)
	    {
	      int temp = cache[count]->lruCount;
	      cache[count]->lruCount = temp+1;
	      return cache[count]->fileContents;
	    }
	}
    }
  return NULL;
}




/*
 * help() - Print a help message
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS]\n", progname);
    printf("Initiate a network file server\n");
    printf("  -l    number of entries in cache\n");
    printf("  -p    port on which to listen for connections\n");
}

/*
 * die() - print an error and exit the program
 */
void die(const char *msg1, char *msg2) {
    fprintf(stderr, "%s, %s\n", msg1, msg2);
    exit(0);
}
/*
 * open_server_socket() - Open a listening socket and return its file
 *                        descriptor, or terminate the program
 */
int open_server_socket(int port) {
    int                listenfd;    /* the server's listening file descriptor */
    struct sockaddr_in addrs;       /* describes which clients we'll accept */
    int                optval = 1;  /* for configuring the socket */

    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("Error creating socket: ", strerror(errno));

    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int)) < 0)
        die("Error configuring socket: ", strerror(errno));

    /* Listenfd will be an endpoint for all requests to the port from any IP
       address */
    bzero((char *) &addrs, sizeof(addrs));
    addrs.sin_family = AF_INET;
    addrs.sin_addr.s_addr = htonl(INADDR_ANY);
    addrs.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (struct sockaddr *)&addrs, sizeof(addrs)) < 0)
        die("Error in bind(): ", strerror(errno));

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, 1024) < 0)  // backlog of 1024
        die("Error in listen(): ", strerror(errno));

    return listenfd;
}

/*
 * handle_requests() - given a listening file descriptor, continually wait
 *                     for a request to come in, and when it arrives, pass it
 *                     to service_function.  Note that this is not a
 *                     multi-threaded server.
 */
void handle_requests(int listenfd, void (*service_function)(int, int), int param) {
    while (1) {
        /* block until we get a connection */
        struct sockaddr_in clientaddr;
        int clientlen = sizeof(clientaddr);
        int connfd;
        if ((connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen)) < 0)
            die("Error in accept(): ", strerror(errno));

        /* print some info about the connection */
        struct hostent *hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                           sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hp == NULL) {
            fprintf(stderr, "DNS error in gethostbyaddr() %d\n", h_errno);
            exit(0);
        }
        char *haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);

        /* serve requests */
        service_function(connfd, param);

        /* clean up, await new connection */
        if (close(connfd) < 0)
            die("Error in close(): ", strerror(errno));
    }
}


void putcFile(int connfd, char *filename)
{
 
    char bufx[1];
    char *sizer = malloc(10);
    int sizerC=0;
    //handles the size line 
    while(1){
      read(connfd,bufx,1);

      sizer[sizerC]=bufx[0];
      sizerC++;

      if(bufx[0] == '\n')
	{
	  break;
	}
    }
    printf("%s",sizer);
    int fcSize=atoi(sizer);
    char *nodeFc=malloc(fcSize);
    int i =0;
    char bufv[1];
    char *hash1=malloc(33);
    int hashSize = 33;

    while(1)
      {
	read(connfd,bufv,1);
	if(bufv[0]=='\n')
	  break;
	hash1[i] = bufv[0];
	i++;
      }
    char* hash = malloc(33);
    strncpy(hash,hash1,33);
    hash[32]='\0';


    //printf("size of hash = %d\n",strlen(hash));
    puts(hash);
    
    
    int flagErrorGuy =0;
    remove(filename);
    FILE *fp = fopen (filename,"ab+");
    if(fp == NULL)
      {
	     flagErrorGuy=1;
      }
    
    read(connfd,nodeFc,fcSize);
    fwrite(nodeFc,fcSize,1,fp);
    printf("%s",nodeFc);
    if(finalLru>0)
      {
	       addNode(nodeFc,filename);
      }

    rewind(fp);

    unsigned char c[16];

    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    MD5_Init (&mdContext);
    while((bytes = fread(data,1, 1024,fp)) != 0){
     MD5_Update( &mdContext,data,bytes);
    }
    MD5_Final (c, &mdContext);
    
    int k;
    char *hash420=(char*)malloc(33);

    for(k=0;k<16;++k)
      {
	       snprintf(&(hash420[k*2]), 16*2, "%02x",(unsigned int)c[k]);
      }
    
    //puts(hash420);

    fclose(fp);
    
    
    if(strcmp(hash420,hash) !=  0){
      flagErrorGuy=1;
    }
   
    //now we want to tell the client OK or an error
    if(flagErrorGuy!=0)
      {
	       char *words = "You got a hash error\n";
	       write(connfd,words,strlen(words));
      }
    else
      write(connfd,"OKC\n",4);
   
    free(hash);
    free(hash420);
}

/*
 * file_server() - Read a request from a socket, satisfy the request, and then close the connection.
 */
void putFile(int connfd, char *filename)
{
    
  //  char bufx[1];
  char *sizer = malloc(10);
  int scounter = 0;
  while(1){
    char *bufx = malloc(1);
    read(connfd,bufx,1); 
    sizer[scounter]=bufx[0];
    scounter++;
    
    if(bufx[0] == '\n')
      {
	free(bufx);
	break;
      }
    free(bufx);
  }
  printf("%s",sizer);
  int fcSize=atoi(sizer);
  free(sizer);
  //printf("size= %d\n",fcSize);
  char *nodeFc = malloc(fcSize);
  int flagErrorGuy =0;
  
  remove(filename);
  FILE *fp = fopen (filename,"ab+");
  if(fp == NULL)
    {
      flagErrorGuy=1;
    }
  
  read(connfd,nodeFc,fcSize);
  fwrite(nodeFc,fcSize,1,fp);
  printf("%s",nodeFc);
  fclose(fp);
  
  if(finalLru>0)
    {
      addNode(nodeFc,filename);
    }
  free(nodeFc);
  //now we want to tell the client OK or an error
  if(flagErrorGuy!=0)
    {
      char *words = "You got an error somewhere, bro (or dud-ette)\n";
      write(connfd,words,strlen(words));
      //free(words);
    }
  else
    write(connfd,"OK\n",3);
  
}

void getcFile(int connfd, char *filename)
{
  FILE *filefd = fopen(filename, "rb");
  int onDiskNotCache=0;
  int onCache = 0;
  //if not on server cache
  if(searcher(filename)==NULL) 
    {
      //if also not on server disk
      if(fopen(filename,"rb")==NULL)
	{
	  char *errm="File doesnt exist on the server disk nor cache\n";
	  write(connfd,errm,strlen(errm));
	  //exit(EXIT_FAILURE);
	  return;
	}
      //if not on cache but is on disk
      else
	{
	  onDiskNotCache=1; 
	  char *okm="OKC\n";
	  write(connfd,okm,strlen(okm));
	}
    }
  //file exists on cache
  else
    {
      onCache=1;
      char *okm="OKC\n";
      write(connfd,okm,strlen(okm));
    }
  if(onCache==1)
    {
      char* fc;
      fc=malloc(strlen(searcher(filename)));
      fc=searcher(filename);
      

      strcat(filename,"\n");
      write(connfd,filename,strlen(filename));
      int size = strlen(fc);
      char sizeS[10];
      //itoa(size,sizeS,10);
      snprintf(sizeS,10,"%d",size);
      strcat(sizeS,"\n");
      write(connfd,sizeS,strlen(sizeS));
    



      unsigned char c[16];

      MD5_CTX mdContext;
      int bytes=strlen(fc);
      //unsigned char data[1024];
      MD5_Init (&mdContext);

      if(bytes != 0){
        MD5_Update( &mdContext,fc,bytes);
      }
      MD5_Final (c, &mdContext);

      int i;
      char *hash420=(char*)malloc(33);

      for(i=0; i<16;++i)
        {
          snprintf(&(hash420[i*2]), 16*2, "%02x", (unsigned int)c[i]);
        }
      strcat(hash420,"\n");
     
      // puts(hash420);




      write(connfd,hash420,strlen(hash420));
      write(connfd,fc,strlen(fc));

      return;

    }


  else if(onDiskNotCache==1)
    {
      unsigned char c[16];
      
      MD5_CTX mdContext;
      int bytes;
      unsigned char data[1024];
      MD5_Init (&mdContext);
      
      while((bytes = fread(data,1, 1024,filefd)) != 0){
	MD5_Update( &mdContext,data,bytes);
      }
      MD5_Final (c, &mdContext);
      
      int i;
      char *hash420=(char*)malloc(33);
      
      for(i=0; i<16;++i)
	{
	  snprintf(&(hash420[i*2]), 16*2, "%02x", (unsigned int)c[i]);
	}
      strcat(hash420,"\n");
      //puts(hash420);
      rewind(filefd);
      
      
      char buffer[BUFSIZ];
      struct stat sb;
      
      
      char *temp = malloc(sizeof(filename));
      strcpy(temp,filename);
      strcat(temp,"\n");
      write(connfd,temp,strlen(temp));
      free(temp);
      
      
      if (stat(filename, &sb) == -1) 
	{
	  perror("stat");
	  //exit(EXIT_FAILURE);
	  return;
	}
      
      char str[256];
      size_t s = sb.st_size;
      
      snprintf(str, sizeof(str),"%zu",s);
      char *temp2 = malloc(sizeof(str));
      strcpy(temp2, str);
      strcat(temp2,"\n");
      
      write(connfd,temp2,strlen(temp2));
      free(temp2);
      
      write(connfd, hash420, strlen(hash420));
      //we should free hash420 at some point
      
      char* contents = malloc(s);
      fread(contents,s,1,filefd);
      write(connfd,contents,s);
      
      //should we get rid of this?
      // if(finalLru>0)
      //{
      //	 addNode(contents,filename);
      //}
      
      fclose(filefd);
    }
}

void getFile(int connfd, char *filename)
{

  char *fc;
  //cache and file is in cache
  if(finalLru>0 && searcher(filename)!=NULL)
    {
      fc=malloc(strlen(searcher(filename)));
      fc=searcher(filename);
      write(connfd,"OK\n",3);
      strcat(filename,"\n");
      write(connfd,filename,strlen(filename));
      int size = strlen(fc);
      char sizeS[10];
      //itoa(size,sizeS,10);
      snprintf(sizeS,10,"%d",size);
      strcat(sizeS,"\n");
      
      write(connfd,sizeS,strlen(sizeS));
      write(connfd,fc,strlen(fc));
      //free(fc);
      return;
    }
  
  //printf("filename in get= %s and length =%d",filename,strlen(filename));
  //printf("getFile says filename = %s\n",filename);
  int filefd = open(filename, O_RDONLY);
  
  if(filefd == -1) {
    char *words = "You've hit an error\n";
    write(connfd,words,strlen(words));
    return;
  }
  
  else {
    char buf[3];
    strcpy(buf,"OK\n");
    write(connfd, buf, 3);
  }
  
  
  char buffer[BUFSIZ];
  struct stat sb;
  
  char *temp = malloc(strlen(filename));
  strcpy(temp,filename);
  strcat(temp,"\n");
  write(connfd,temp,strlen(temp));
  free(temp);
  
  
  
  if (stat(filename, &sb) == -1)
    {
      perror("stat");
      //exit(EXIT_FAILURE);
      return;
    }
  
  char str[256];
  size_t s = sb.st_size;
  char *contents=malloc(s);
  
  
  
  snprintf(str, sizeof(str),"%zu",s);
  char *temp2 = malloc(strlen(str));
  strcpy(temp2, str);
  strcat(temp2,"\n");
  
  write(connfd,temp2,strlen(temp2));
  free(temp2);
  
  read(filefd,contents,s);
  write(connfd,contents,s);
  free(contents);
  //should we get rid of this???????????????
  //if(finalLru>0)
  //addNode(contents,filename);
  
  close(filefd);   
}



void file_server(int connfd, int lru_size) {

  char *filename1=malloc(20);
  // printf("size of filename to start  = %d\n",strlen(filename1));

  char* holder1=malloc(5);
  int counter=0;
  while(1)
    {
      char *bufd  = malloc(1);
      read(connfd,bufd,1);
      if(counter==0 && bufd[0]=='E')
	{
	  return;
	}
      //printf("%s",bufd);
      holder1[counter]=bufd[0];
      if(bufd[0] == '\n')
	{
	  free(bufd);
	  break;
	}
      //holder[counter] = bufd[0];
      //	strcat(holder,bufd);
      free(bufd);
      counter++;
    }

  //printf("%d",strlen(holder1));
  char* holder = malloc(counter);
  strncpy(holder,holder1,counter+1);
  holder[counter]='\0';
  free(holder1);
  //printf("size of holder= %d\n",strlen(holder));
  printf("%s\n",holder);
  
  // char *filename=malloc(256); //will hold the filename
  //char bufb[1];
  int i = 0;
  
  //printf("size of filename before the loop = %d\n",strlen(filename1));
     
    while(1){
      char *bufb =malloc(1);
      read(connfd,bufb,1); 
      //printf("%s",bufb);
      filename1[i]=bufb[0];
      
      if(bufb[0] == '\n')
	{
	  free(bufb);
	  break;
	}
      //puts(bufb);
      //filename[i] = bufb[0];
      free(bufb);
      i++;
    }
    
    //printf("i = %d\n",i);
    //printf("length of filename= %d\n",strlen(filename1));
    //printf("%s\n",filename1);
    //free(filename1);
    char* filename = malloc(i);
    strncpy(filename,filename1,i+1);
    filename[i]='\0';
    
    //int y;
    /*for(y=0; y<i; y++)
      {
	filename[y]=filename1[y];
	}*/
    free(filename1);

    //printf("filename = %s and its length is %d\n",filename,strlen(filename));
    printf("%s\n",filename);
    if(holder[0]=='P' && holder[3] != 'C')
      {
	putFile(connfd,filename);
	filename=NULL;
	free(filename);
	free(holder);
      }
    
    else if(holder[0]=='G' && holder[3] != 'C')
      { 
	getFile(connfd,filename);
	filename=NULL;
	free(filename);
	free(holder);
      } 
    else if(holder[0] == 'P' && holder[3] == 'C')
      {
	putcFile(connfd,filename);
	filename=NULL;
	free(filename);
	free(holder);
      }
    else if(holder[0] == 'G' && holder[3] == 'C')
      {
	getcFile(connfd,filename);
	filename=NULL;
	free(filename);
	free(holder);
      }
    else
      {
	perror("Wasnt PUT, PUTC, GET, or GETC");
	free(holder);
	free(filename);

      }
    //free(holder);
    //free(filename);
}


/*
 * main() - parse command line, create a socket, handle requests
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;
    /* NB: the "part 3" behavior only should happen when lru_size > 0 */
    int  lru_size = 0;
    int  port     = 9000;

    check_team(argv[0]);

    /* parse the command-line options.  They are 'p' for port number,  */
    /* and 'l' for lru cache size.  'h' is also supported. */
    while ((opt = getopt(argc, argv, "hl:p:")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
          case 'l': lru_size = atoi(optarg); break;
          case 'p': port = atoi(optarg); break;
        }
    }
    //finalLru=lru_size;
    //printf("%d\n",finalLru);
    finalLru=lru_size;
    if(finalLru>0)
      cache=(malloc(finalLru * ((sizeof(struct Node))+sizeof(char *)+sizeof(char *)+sizeof(int))));

    /* open a socket, and start handling requests */
    int fd = open_server_socket(port);
    handle_requests(fd, file_server, lru_size);
    free(cache);
    exit(0);
}
