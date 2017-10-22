all:
	 gcc -O3 ./bsfuck.c strchunk.c examples_common.c miscdata/slist.c dumb-rb/ringbuf.c -o bsfuck
