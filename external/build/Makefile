SRCS = \
    $(CURDIR)/lz4/lib/lz4.c \

OBJS = $(SRCS:.c=.o)

TARGET = $(CURDIR)/lz4/liblz4.a

.c.o:
	clang -c -O2 -W -Wall -Wextra -std=c++11 -stdlib=libc++ -pedantic -o $@ $<

$(TARGET): $(OBJS)
	ar rsv $@ $^
	@mkdir -p $(CURDIR)/lib
	@mkdir -p $(CURDIR)/include
	@mv $(TARGET) $(CURDIR)/lib
	@cp -a $(CURDIR)/lz4/lib/lz4.h $(CURDIR)/include

clean:
	rm $(OBJS) $(TARGET)
