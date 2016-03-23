libjieba.so:
	gcc -o sqljieba.o -c -fPIC -I/usr/local/mysql/include/ sqljieba.c
	g++ -o jieba.o -c -DLOGGING_LEVEL=LL_WARNING -fPIC -I./deps/ jieba.cpp
	gcc -shared -fPIC -o libjieba.so jieba.o
clean:
	rm -f *.so *.o
