libjieba.so:
	gcc -o sqljieba.o -c -fPIC -I../../../include/ -I../../../build/include -I../../../libbinlogevents/export sqljieba.c
	g++ -o jieba.o -c -DLOGGING_LEVEL=LL_WARNING -fPIC -I./deps/ jieba.cpp
	gcc -shared -fPIC -o libjieba.so jieba.o
clean:
	rm -f *.so *.o
