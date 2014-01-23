target: name type

name: main.c
	gcc -g -pedantic -Wall -Wextra -Werror main.c -o name

type: type.c
	gcc -g -pedantic -Wall -Wextra -Werror type.c -o type -lm

