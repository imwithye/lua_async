lua_async: lua_async.c async.c
	gcc -luv -llua -Wall -fPIC -o2 -o lua_async lua_async.c async.c

test: lua_async
	./lua_async example.lua

clean:
	git clean -Xdf

.PHONY: clean