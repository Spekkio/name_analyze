#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

/*'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'*/

const char con[] = {'B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','V','W','X','Z'};
const char voc[] = {'A','E','I','O','U','Y'};

char c;
char * buf = &c;

char buffer[5];
char typebf[5];
char namebf[255];
char tnmebf[255];

void rotate(const char n)
{
    buffer[0] = buffer[1];
    buffer[1] = buffer[2];
    buffer[2] = buffer[3];
    buffer[3] = n;
}

void reset(void)
{
    buffer[0] = '_';
    buffer[1] = '_';
    buffer[2] = '_';
    buffer[3] = '_';
    buffer[4] = '\0';
}

int isEmpty(void)
{
  return ((buffer[0]=='_') && (buffer[1]=='_') && (buffer[2]=='_') && (buffer[3]='_'));
}

int isCon(const char c)
{
  int i;
  i=0;
  while(i<20) {
    if(c == con[i]) return 1;
    i++;
  }
  return 0;
}

int isVoc(const char c)
{
  int i;
  i=0;
  while(i<6) {
    if(c == voc[i]) return 1;
    i++;
  }
  return 0;
}

char * convert(char * out, const char * in, const int len)
{
  int i;
  i=0;
  while(i<len) {
    if(isCon(in[i])) {
      out[i] = 'C';
    }
    else if(isVoc(in[i])) {
      out[i] = 'V';
    } else out[i] = '_';
    i++;
  }
  return out;
}

int main(void)
{
  int r;
  FILE * f;
  char l;

  f = fopen("names.txt","r");

  if(f!=0)
    {
      l=1;
      r=0;
      reset();
      while(l!=0) {
	l=fread(buf,1,1,f);
	if(!(c=='\n' || c=='\r'))
	  {
	    rotate(c);
	    namebf[r++] = c;
	    /*printf("%s -> %s\n",&buffer[0],convert(typebf,buffer,4));*/
	  }
	else
	  {
	    reset();
	    namebf[r++]='\0';
	    /*printf("name: %s -> %s\n",&namebf[0],convert(tnmebf,namebf,r-1));*/
	    printf("%s\n",convert(tnmebf,namebf,r-1));
	    r=0;
	  }
      }
    }

  fclose(f);
  return 0;
}
