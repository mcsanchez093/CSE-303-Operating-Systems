#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
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

#define PADDING RSA_PKCS1_PADDING

int checkSum = 0;
int encryptFlag = 0;


/*
 * help() - Print a help message
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS]\n", progname);
    printf("Perform a PUT or a GET from a network file server\n");
    printf("  -P    PUT file indicated by parameter\n");
    printf("  -G    GET file indicated by parameter\n");
    printf("  -C    use checksums for PUT and GET\n");
    printf("  -e    use encryption, with public.pem and private.pem\n");
    printf("  -s    server info (IP or hostname)\n");
    printf("  -p    port on which to contact server\n");
    printf("  -S    for GETs, name to use when saving file locally\n");
}

/*
 * die() - print an error and exit the program
 */
void die(const char *msg1, char *msg2) {
    fprintf(stderr, "%s, %s\n", msg1, msg2);
    exit(0);
}

/*
 * connect_to_server() - open a connection to the server specified by the
 *                       parameters
 */
int connect_to_server(char *server, int port) {
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    char errbuf[256];                                   /* for errors */

    /* create a socket */
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("Error creating socket: ", strerror(errno));

    /* Fill in the server's IP address and port */
    if ((hp = gethostbyname(server)) == NULL) {
        sprintf(errbuf, "%d", h_errno);
        die("DNS error: DNS error ", errbuf);
    }
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0],
          (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    /* connect */
    if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        die("Error connecting: ", strerror(errno));
    return clientfd;
}

/*********************ENCRPTION CODE BELOW***************************/

RSA * createRSAWithFilename(char *filename, int public){

  
  FILE *fp = fopen(filename,"r");

  if(fp == NULL){
    perror("RSA file open issue");
    return NULL;
  }
  RSA *rsa = RSA_new();;

  if(public){
     puts("setting rsa");
     RSA *rs = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
     puts("rsa set");
     return rs;
 }
  else{
   RSA  *rs = PEM_read_RSAPrivateKey(fp, &rsa,NULL,NULL);
   return rs;
}
  puts("returning from function");
  // return rsa;
}








/*
RSA * createRSA(unsigned char * key, int public){
  RSA *rsa = NULL;
  //puts(key);
  BIO *keybio = BIO_new_mem_buf(key,-1);
  if(keybio==NULL){
    printf("Failed to key BIO");
    return 0;
  }
  if(public==1){
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, NULL,NULL, NULL);

    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA\n");
    }
 
    return rsa;
}*/


int public_encrypt(unsigned char * content, int content_len, char * key, unsigned char * encrypted){
  RSA * rsa = createRSAWithFilename("public.pem",1);
  int rsaLen = RSA_size(rsa);
  printf("rsaLen= %d\n",rsaLen);
  encrypted = (unsigned char *)malloc(rsaLen);
  
  puts("in public encrypt");
  int result = RSA_public_encrypt(content_len,content,encrypted,rsa,PADDING);
  printf("%s\n",encrypted);
  //\printf("%d\n",result);
  puts("about to return from public_encrypt");
  return result;
}

int public_decrypt(unsigned char * enc_data,int data_len,char * filename, unsigned char *decrypted)
{
    RSA * rsa = createRSAWithFilename(filename,1);
    int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,PADDING);
    return result;
}

  
int private_encrypt(unsigned char * data,int data_len,char * filename, unsigned char *encrypted)
{
    RSA * rsa = createRSAWithFilename(filename,0);
    int result = RSA_private_encrypt(data_len,data,encrypted,rsa,PADDING);
    return result;
}

int private_decrypt(unsigned char * enc_data,int data_len,char * filename, unsigned char *decrypted)
{
    RSA * rsa = createRSAWithFilename("private.pem",0);

    int rsaLen = RSA_size(rsa);
    printf("rsaLen= %d\n",rsaLen);
    decrypted = (unsigned char *)malloc(rsaLen);
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,PADDING);
    puts(decrypted);
    printf("%d\n",result);
    return result;

}


/*************************END OF ENCRYPTION CODE***************************/

void put_file(int fd, char *put_name){
   
    char buffer[BUFSIZ];
    struct stat sb, puf,pvf;
    char *hash420;
    unsigned char *publicKey;
    unsigned char *privateKey;
    unsigned char  encrypted[4098]={};
    unsigned char decrypted[4098]={};

    FILE *filefd = fopen(put_name, "rb");


    if(filefd == NULL)
      {
	write(fd,"E",1);
	perror("put_file error in opening");
	exit(EXIT_FAILURE);
      }
    
    if(checkSum == 1)
      {
	unsigned char c[16];
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	MD5_Init (&mdContext);
	
        while((bytes = fread(data,1, 1024,filefd)) != 0)
	  {
	    MD5_Update( &mdContext,data,bytes);
	  }
	MD5_Final (c, &mdContext);
        
	int i;
	hash420=(char*)malloc(33);
	
	for(i=0; i<16;++i)
	  {
	    snprintf(&(hash420[i*2]), 16*2, "%02x", (unsigned int)c[i]);
	  }
	strcat(hash420,"\n");
	//       puts(hash420);
	rewind(filefd);
      }
    
    if(checkSum == 1)
      {
	write(fd,"PUTC\n",5);
      }
    else
      {
	write(fd,"PUT\n",4);
      }
    
    
    //puts("before  filename write");
    //filename write
    char *temp = malloc(strlen(put_name));
    strcpy(temp,put_name); 
    strcat(temp,"\n");
    
    write(fd,temp,strlen(temp));
    //puts("write to fd worked");
    
    free(temp);
    
    //puts("before  size write");
    //size of file    
    if (stat(put_name, &sb) == -1) {
      perror("stat");
      exit(EXIT_FAILURE);
    }
    char str[256];
    size_t s = sb.st_size;
    
    snprintf(str, sizeof(str),"%zu",s);  
        
    char *temp2 = malloc(strlen(str));
    strcpy(temp2, str);
    strcat(temp2,"\n");
    
    write(fd,temp2,strlen(temp2));
    free(temp2);

     //add checksum if needed
    if(checkSum == 1)
      {
	//printf("hash420= %s and length= %d\n",hash420,strlen(hash420));
	write(fd,hash420,strlen(hash420)); 
      } 
    //puts("reading contents of file below");
    int read_return = fread(buffer,s,1,filefd);
    //puts(buffer);
    /*if(read_return == 0){
      break;
      }
      if(read_return == -1){
      perror("read");
      exit(EXIT_FAILURE);
      }*/
    
    //writing file contents?
    if(encryptFlag == 1)
      {
	puts("encrypting file below");
	//printf("%d\n",s);
	char toEnc[256];
	strcpy(toEnc,buffer);
        puts(toEnc);
        /*
	  int diff=256-s;
	  while(diff>0){
          strcat(toEnc,"w");
          diff--;
	  }*/
	//puts(toEnc);
	/*
	  if (stat("public.pem", &puf) == -1) {
	  perror("stat");
	  exit(EXIT_FAILURE);
	  }
	  
	  char str1[256];
	  size_t a = puf.st_size;
	  
	  snprintf(str1, sizeof(str1),"%zu",a); 
	  
	  publicKey = malloc(strlen(str1));
	  
	  //char *contents = malloc(strlen(size));
	  //int sI = atoi(a);
	  int pub = open("public.pem",O_RDONLY);
	  read(pub,publicKey,a);
	  close(pub);*/
	//puts(publicKey);
	int encrypted_length= public_encrypt(toEnc,strlen(toEnc),"public.pem",encrypted);
	if(encrypted_length == -1)
	  {
	    printf("Public Encrypt failed\n");
	    exit(0);
	  }
        printf("Encrypted length =%d\n",encrypted_length);
	
	int decrypted_length = private_decrypt(encrypted,encrypted_length,"private.pem", decrypted);
	if(decrypted_length == -1)
	  {
            puts("Private Decrypt failed ");
            exit(0);
	  }
	
	puts(decrypted);
	
	if (stat("private.pem", &pvf) == -1)
	  {
	    perror("stat");
	    exit(EXIT_FAILURE);
	  }
	/*   char str2[256];
	     size_t q = pvf.st_size;
	     
	     snprintf(str2, sizeof(str2),"%zu",q); 
	     
	     privateKey = malloc(strlen(str2));
	     
	     //nt sQ = atoi(q);
	     int priv = open("private.pem",O_RDONLY);
	     read(priv,privateKey,q);
	     //puts(privateKey);
	     close(priv);
	*/
	//puts(publicKey);
	//int encrypted_length= public_encrypt(toEnc,strlen(toEnc),publicKey,encrypted);
	//puts("encrypted file");
      }
    /*if(encrypted_length == -1)
      {
      puts("Public Encrypt failed ");
      exit(0);
      }
      printf("Encrypted length =%d\n",encrypted_length);
      printf("Encrypted text =%s\n",encrypted);
      
      int decrypted_length = private_decrypt(encrypted,encrypted_length,"private.pem", decrypted);
      if(decrypted_length == -1)
      {
      puts("Private Decrypt failed ");
      exit(0);
      }
      printf("Decrypted Text =%s\n",decrypted);
      printf("Decrypted Length =%d\n",decrypted_length);
      }*/
    
    //else{
    if(write(fd, buffer, s) == -1)
      {
	perror("write");
	exit(EXIT_FAILURE);
      }
    //}
    //puts("before fclose(filefd)");
    fclose(filefd);
    
    //handles error or OK
    char buf[1];
    while(1)
      {
	read(fd,buf,1);
	if(buf[0] == '\n'){
	  break;
	}
        printf("%s",buf);
      }
    
    puts("");
    
    exit(EXIT_SUCCESS);
    
}



void get_file(int fd, char *get_name,char *save_name){
    struct stat sb;
  

   if(checkSum == 1){
    write(fd,"GETC\n",5);
   }
   else{
    write(fd,"GET\n",4);
   }

   //filename
   
   char *temp = malloc(strlen(get_name));
   strcpy(temp,get_name); 
   strcat(temp,"\n");
   //   printf("temp= %s and its size =%d\n",temp,strlen(temp));
   write(fd,temp,strlen(temp));
   //printf("Client get_name size = %d\n",strlen(temp));
   free(temp);

    //remove error Test
    char bufb[1];
    char errorTest[50];
    int j = 0;
    while(1){
        read(fd,bufb,1);
        errorTest[j] = bufb[0];
        if(bufb[0] == '\n'){
            break;
        }
        j++;
    }
    printf("%s",errorTest);

    if(errorTest[0]!='O')
      exit(0);


    //remove name
   char name[50];
    char bufx[1];
    int k = 0;
     while(1){
         read(fd,bufx,1);
         name[k] = bufx[0];
         if(bufx[0] == '\n'){
            break;
         }   
         k++;
     }
     printf("%s",name);

     //removes size;
     char size1[10];
     int w = 0;
    while(1){
            read(fd,bufx,1); 
            size1[w]=bufx[0];
            if(bufx[0] == '\n'){
                break;
            }
            w++;
        }
    //printf("w= %d",w);
    //printf("size is actually = %s\n",size1);
    char* size = malloc(w+1);
    strncpy(size,size1,w+1);
    size[w]='\0';

    //printf("Size= %s",size);
    puts(size);


    //reading hash
    char *hash=malloc(33);
    if(checkSum == 1)
      {
	//char *hash=malloc(33);
	int hashSize = 33;
	int q = 0;
	char bufv[1];
	while(1)
	  {
	    read(fd,bufv,1);
	    hash[q] = bufv[0];
	    if(bufv[0]=='\n')
	      break;
	    q++;
	  }
      }
   
    



    //char *contents = malloc(strlen(size));
    int s = atoi(size);
    char* contents = malloc(s);
    read(fd,contents,s);
   /*printf("%s",contents);
    int fp = open(save_name, O_RDWR | O_APPEND);
    if ((fp == -1))
      {
        fp = open(save_name, O_RDWR | O_APPEND|O_CREAT | O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
      }
    int writeVal = write(fp,contents,s);
   */

    if(checkSum == 1)
      {   
	//fseek(fp, 0, SEEK_SET);
	//FILE *fq = fdopen (fp,"rb");
	//rewind(fq);
	
	unsigned char c[16];
	
	MD5_CTX mdContext;
	int bytes=s;
	//unsigned char data[1024];
	MD5_Init (&mdContext);
	if(bytes != 0){
	  MD5_Update( &mdContext,contents,bytes);
	}
	MD5_Final (c, &mdContext);
	
	int a;
	char *hash420=(char*)malloc(33);
	
	for(a=0;a<16;++a)
        {
	  snprintf(&(hash420[a*2]), 16*2, "%02x",(unsigned int)c[a]);
        }
	strcat(hash420,"\n");
	//fclose(fq);
	//puts(hash);
	//puts("other hash now:");
	//puts(hash420);
	//printf("%d\n",strlen(hash));
	//printf("%d\n",strlen(hash420));

	if(strcmp(hash,hash420)!=0)
	  {
	    puts("Hashs do not match");
	    free(hash420);
	    free(hash);
	    exit(0);
	  }
	else
	  printf("%s",hash);

	free(hash);
	free(hash420);
	//fclose(fq);
      }
    //puts("");
    printf("%s",contents);
    remove(save_name);
    FILE* f = fopen(save_name,"w+");
    //printf("size of contents = %d and size of s = %d\n",strlen(contents),s);
    fwrite(contents,s,1,f);
    //free(contents);
    //puts("made it to the end");
    fclose(f);    
}



/*
 * main() - parse command line, open a socket, transfer a file
 */
int main(int argc, char **argv) {
    /* for getopt */
    long  opt;
    char *server = NULL;
    char *put_name = NULL;
    char *get_name = NULL;
    int   port;
    char *save_name = NULL;
    char *c_name = NULL;
    char *e_encrypt = NULL;

    check_team(argv[0]);


    /* parse the command-line options. */
    /* TODO: add additional opt flags */
    while ((opt = getopt(argc, argv, "hs:P:G:eS:Cp:")) != -1) {
      switch(opt) {
      case 'h': help(argv[0]); break;
      case 's': server = optarg; break;
      case 'P': put_name = optarg; break;
      case 'G': get_name = optarg; break;
      case 'S': save_name = optarg; break;
      case 'C': c_name = "c"; break;
      case 'p': port = atoi(optarg); break;
      case 'e': e_encrypt = "e"; break;
        }
    }
    


    /* open a connection to the server */
   
    int fd = connect_to_server(server, port);
    



    /* put or get, as appropriate */
    if (put_name!=NULL && c_name == NULL && e_encrypt == NULL){
      put_file(fd, put_name);
    }
    else if (get_name!= NULL && c_name==NULL && e_encrypt == NULL){
      get_file(fd, get_name, save_name);
    }
    else if(put_name!=NULL && c_name !=NULL && e_encrypt == NULL){
      checkSum = 1;
      put_file(fd, put_name);
    }
    else if(get_name!=NULL && c_name !=NULL && e_encrypt == NULL){
      checkSum = 1;
      get_file(fd, get_name, save_name);
    }
    if (put_name!=NULL && c_name == NULL && e_encrypt != NULL){
      encryptFlag = 1;
      put_file(fd, put_name);
    }
    else if (get_name!= NULL && c_name==NULL && e_encrypt != NULL){
      encryptFlag = 1;
      get_file(fd, get_name, save_name);
    }
    else if(put_name!=NULL && c_name !=NULL && e_encrypt != NULL){
      checkSum = 1;
      encryptFlag = 1;
      put_file(fd, put_name);
    }
    else if(get_name!=NULL && c_name !=NULL && e_encrypt != NULL){
      checkSum = 1;
      encryptFlag = 1;
      get_file(fd, get_name, save_name);
    }
    
    /* close the socket */
    int rc;
    if ((rc = close(fd)) < 0)
        die("Close error: ", strerror(errno));
    exit(0);
}
