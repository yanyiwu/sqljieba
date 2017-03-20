# Where MySQL headers are installed
MYSQL_PATH = /usr/include/mysql
# Where MySQL libraries are installed
MYSQL_LIB_PATH = /usr/lib/mysql

CPPFLAGS = -DMYSQL_DYNAMIC_PLUGIN -DMYSQL_ABI_CHECK -DLOGGING_LEVEL=LL_WARNING
CPPFLAGS += -I./deps/ -I$(MYSQL_PATH)

libsqljieba.so:
	g++ -O2 -fPIC -o sqljieba.o $(CPPFLAGS) -c sqljieba.cpp
	g++ -shared -fPIC -o libsqljieba.so sqljieba.o

clean:
	rm -f *.so *.o

install:
	cp libsqljieba.so $(MYSQL_LIB_PATH)/plugin/
	cp -r ./dict /usr/share/
