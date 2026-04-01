NAME  := codexion
CC    := cc
FLAGS := -Wall -Wextra -Werror -Icoders -pthread

SRCS  := \
	coders/main.c \
	coders/args.c \
	coders/time.c \
	coders/log.c \
	coders/coder.c \
	coders/init.c \
	coders/dongle.c
OBJS  := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re