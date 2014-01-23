#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/*'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'*/

const char con[] = {'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','z'};
const char voc[] = {'a','e','i','o','u','y'};

char c;
char * buf = &c;

char buffer[4];

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
}

int main(void)
{
    FILE * f;
    char l;

    f = fopen("names.txt","r");


    if(f!=0)
        {
            l=1;
            reset();
            while(l!=0) {
                l=fread(buf,1,1,f);

                if(c!='\n' || c!='\r')
                    {
                        rotate(c);
                    } else reset();

                printf("%s\n",&buffer[0]);
            }
        }

    fclose(f);
    return 0;
}
