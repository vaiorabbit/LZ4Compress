SRCS = \
    Main.cpp \
    Option.cpp \
    Convert.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = LZ4Compress

.cpp.o:
	clang++ -c -O2 -W -Wall -Wextra -std=c++11 -stdlib=libc++ -pedantic -I ../external/include -o $@ $<
#	g++ -c -O2 -W -Wall -Wextra -ansi -pedantic -I ../external/zopfli -o $@ $<

$(TARGET): $(OBJS) ../external/lib/liblz4.a
	clang++ -L ../external/lib -llz4 -stdlib=libc++ -o $@ $^
#	g++ -L ../external/zopfli -lzopfli -o $@ $^
#	cp $(TARGET) ../out

clean:
	rm $(OBJS) $(TARGET)
