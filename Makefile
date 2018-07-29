async_lua: async_lua.c async.c async.h
	gcc -luv -llua -Wall -fPIC -o2 -o async_lua async_lua.c async.c