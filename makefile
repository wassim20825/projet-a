CFLAGS = -Wall -g
LIBS = -lSDL -lSDL_image -lSDL_ttf

EXECUTABLE = pixel_game

SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)
