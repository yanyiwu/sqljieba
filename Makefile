libjieba.so:
	gcc -O2 -o sqljieba.o -c -DMYSQL_DYNAMIC_PLUGIN -DMYSQL_ABI_CHECK -fPIC -I/usr/include/mysql sqljieba.c
	g++ -std=c++11 -O2 -o jieba.o -c -DMYSQL_DYNAMIC_PLUGIN -DLOGGING_LEVEL=LL_WARNING -fPIC -I./deps/ jieba.cpp
	g++ -shared -fPIC -o libsqljieba.so jieba.o sqljieba.o
clean:
	rm -f *.so *.o
install:
	cp libsqljieba.so /usr/lib/mysql/plugin/
	cp -r ./dict /usr/share/
