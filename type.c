#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <math.h>

/*analyze types*/

#define VOC 0
#define CON 1
#define END 2

struct tree_node
{
  float p[3];                        /*probability for each path*/
  unsigned int count[3];
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
NNODES   = sum[3^n, {n, 0, N-1}]
NNODES2  = sum[3^n, {n, 0, N-2}]
N=number of chars on longest name

N=10 gives 88573 tree_nodes
*/

#define DEPTH   10

#define NNODES  88573
#define NNODES2 29524

#define BOTTOM 29524 /*3^(DEPTH-1), bottom row nodes*/

struct tree_node nodes[NNODES];

char c;
char * buf = &c;

void setupNodes(void)
{
  /*
  unsigned long int c,p,i,r;
  char l;
  l=DEPTH-2;
  i=0;
  r=0;
  while(l>=0) {
    p = powl(3,l);
    c=0;
    while(c<p) {
      nodes[r].p[VOC] = 0.0;
      nodes[r].p[CON] = 0.0;
      nodes[r].p[END] = 0.0;

      nodes[r].count[VOC] = NNODES-i-0;
      nodes[r].count[CON] = NNODES-i-1;
      nodes[r].count[END] = NNODES-i-2;

      nodes[r].tree_node[VOC] = &nodes[NNODES-i-0];
      nodes[r].tree_node[CON] = &nodes[NNODES-i-1];
      nodes[r].tree_node[END] = &nodes[NNODES-i-2];

    c++;
    r++;
    i+=3;
  }
  l--;
  }
  */
  unsigned long int i,c;

  c=0;i=1;
  while(c<NNODES2) {
    nodes[c].p[VOC] = 0.0;
    nodes[c].p[CON] = 0.0;
    nodes[c].p[END] = 0.0;

    nodes[c].count[VOC] = 0;
    nodes[c].count[CON] = 0;
    nodes[c].count[END] = 0;

    nodes[c].tree_node[VOC] = &nodes[i++];
    nodes[c].tree_node[CON] = &nodes[i++];
    nodes[c].tree_node[END] = &nodes[i++];

    c++;
  }

}

int main(void)
{
  unsigned int i, l;
  float sum;
  struct tree_node * n;
  FILE * f;
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

    printf("node: %u -> %.3f %.3f %.3f\n",i,nodes[i].p[0]*100,nodes[i].p[1]*100,nodes[i].p[2]*100);
    sum=0;
    i++;
  }

  fclose(f);

  return 0;
}
