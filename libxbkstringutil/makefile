LIBVERSION=1
LIBSUBVERSION=0.0

all:
	cd src; \
	c++ -fPIC -Wall -g -c xbkstringutil.cpp; \
	c++ -g -shared -WL,-soname,libxbkstringutil.so.${LIBVERSION} -o libxbkstringutil.so.${LIBVERSION}.${LIBSUBVERSION} xbkstringutil.o -lc; \
	strip lib*; \
	cp lib* /usr/local/lib;
	-ln -sf /usr/local/lib/libxbkstringutil.so.${LIBVERSION}.${LIBSUBVERSION} /usr/lib/libxbkstringutil.so.${LIBVERSION};
	ln -sf /usr/lib/libxbkstringutil.so.${LIBVERSION} /usr/lib/libxbkstringutil${LIBVERSION}.so
	-mkdir /usr/include/xbk
	-cp include/*.h /usr/include/xbk	
	
