spchk: traverse.c dictionary.c readfile.c driver.c
	gcc -o spchk -fsanitize=address -Wall -g traverse.c dictionary.c readfile.c driver.c

checkfile: dictionary.c readfile.c
	gcc -o checkfile -fsanitize=address -Wall -g traverse.c dictionary.c readfile.c

traverse: traverse.c
	gcc -o traverse -fsanitize=address -Wall -g traverse.c