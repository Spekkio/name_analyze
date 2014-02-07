#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*analyze types*/

const char con[] = {'B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','V','W','X','Z'}; /*20*/
const char voc[] = {'A','E','I','O','U','Y'}; /*6*/

#define VOC 0
#define CON 1
#define END 2

struct tree_node
{
  float p[3];                        /*probability for each path*/
  unsigned int count[3];
  unsigned long int index[3];
  struct tree_node * tree_node[3];   /*pointers to the next paths*/
}tree_node;

/*
   * <- tree_node
 / | \
V  C  E

3 char name would have,
3 stages, 1 start node,
1 + 3 + 3*3 + 3*3*3 = 40
or
NNODES   = sum[3^n, {n, 0, N}]
NNODES2  = sum[3^n, {n, 0, N-1}]
N=number of chars on longest name

N=10 gives 88573 tree_nodes
*/

#define DEPTH   13

#define NNODES  2391484
#define NNODES2 797161

#define BOTTOM 797161 /*3^(DEPTH-1), bottom row nodes*/

struct tree_node nodes[NNODES];

char c;
char * buf = &c;

void setupNodes(void)
{
  unsigned long int i,c;

  c=0;i=1;
  while(c<NNODES2) {
    nodes[c].p[VOC] = 0.0;
    nodes[c].p[CON] = 0.0;
    nodes[c].p[END] = 0.0;

    nodes[c].count[VOC] = 0;
    nodes[c].count[CON] = 0;
    nodes[c].count[END] = 0;

    nodes[c].index[VOC] = i++;
    nodes[c].index[CON] = i++;
    nodes[c].index[END] = i++;

    nodes[c].tree_node[VOC] = &nodes[nodes[c].index[VOC]];
    nodes[c].tree_node[CON] = &nodes[nodes[c].index[CON]];
    nodes[c].tree_node[END] = &nodes[nodes[c].index[END]];

    c++;
  }

}

#define GENERATE 1
#define PROB     2

int flags=0;

void params(int argc, char **argv)
{
  int i=0;
  while(i<argc)
    {
      if(strncmp(argv[i],"--help",6)==0)
	{
	  printf(" --g\tgenerate a list with random names\n");
	  printf(" --p\tgenerate a probability tree\n");
	  exit(0);
	}
      else if(strncmp(argv[i],"--g",3)==0)
	{
	  flags=0;
	  flags|=GENERATE;
	}
      else if(strncmp(argv[i],"--p",3)==0)
	{
	  flags=0;
	  flags|=PROB;
	}
      i++;
    }
}

int main(int argc, char **argv)
{
  unsigned int i, l,cn;
  float sum;
  struct tree_node * n;
  FILE * f;

  params(argc, argv);

  f=fopen("all_types.txt","r");

  setupNodes();

  n = &nodes[0];
  l=1;

  if(f!=0)
    {
      while(l!=0){
      l=fread(buf, 1,1,f);
      if((!(c=='\n' || c=='\r')) && l!=0)
	{
	  /*printf("%c",c);*/
	  switch(c)
	    {
	    case 'C':
	      n->count[CON]++;
	      n = n->tree_node[CON]; /*next node*/
	      break;

	    case 'V':
	      n->count[VOC]++;
	      n = n->tree_node[VOC]; /*next node*/
	      break;

	    default:
	      break;
	    }
	}
      else {
	/*reset*/
	/*printf("\n");*/
	n->count[END]++;
	n = &nodes[0];
      }
      }
    }

  i=0;
  while(i<NNODES) {
    sum = nodes[i].count[0]+nodes[i].count[1]+nodes[i].count[2];
    nodes[i].p[0] = ((float)nodes[i].count[0])/sum;
    nodes[i].p[1] = ((float)nodes[i].count[1])/sum;
    nodes[i].p[2] = ((float)nodes[i].count[2])/sum;

    if(flags & PROB)
    printf("node: %u -> %.5f(->%lu,N %u) %.5f(->%lu,N %u) %.5f(->%lu,N %u)\n",i,
	   nodes[i].p[0]*100,nodes[i].index[0],nodes[i].count[0],
	   nodes[i].p[1]*100,nodes[i].index[1],nodes[i].count[1],
	   nodes[i].p[2]*100,nodes[i].index[2],nodes[i].count[2]);

    sum=0;
    i++;
  }

  /*test*/
  n = &nodes[0];
  cn=0;

  if(flags & GENERATE)
  while(cn<1000000) {
  l=1;
  n = &nodes[0];
  while(l) {
    i = rand() % 100;

    if(i<(unsigned int)(round(n->p[VOC]*100)))
      {
	n = n->tree_node[VOC]; /*next node*/
	printf("%c",voc[rand() % 5]);
	if(n==0) l=0;
      }
    else if(i<(unsigned int)(round(n->p[VOC]*100 + n->p[CON]*100)))
      {
	n = n->tree_node[CON]; /*next node*/
	printf("%c",con[rand() % 19]);
	if(n==0) l=0;
      }
    else if(i<=(unsigned int)(round(n->p[VOC]*100 + n->p[CON]*100 + n->p[END]*100)))
      {
	l=0;
      }
  }
  printf("\n");
  cn++;
  }


  fclose(f);

  return 0;
}
