libjieba.so:
	gcc -o sqljieba.o -c -DMYSQL_DYNAMIC_PLUGIN -fPIC -I../../../include/ -I../../../build/include -I../../../libbinlogevents/export sqljieba.c
	g++ -o jieba.o -c -DMYSQL_DYNAMIC_PLUGIN -DLOGGING_LEVEL=LL_WARNING -fPIC -I./deps/ jieba.cpp
	gcc -shared -fPIC -o libsqljieba.so jieba.o sqljieba.o
clean:
	rm -f *.so *.o
