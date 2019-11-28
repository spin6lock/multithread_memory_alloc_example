.PHONY: all jemalloc

all:
	gcc main.c -g -o main -lpthread

jemalloc:
	cd jemalloc && ./autogen.sh --with-jemalloc-conf="background_thread:true" --disable-valgrind
