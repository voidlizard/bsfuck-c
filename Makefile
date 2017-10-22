all:
	 gcc -O2 ./bsfuck.c strchunk.c examples_common.c miscdata/slist.c -o bsfuck
