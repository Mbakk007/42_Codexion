NAME := codexion

CC := cc
CFLAGS := -Wall -Wextra -Werror -pthread
INCLUDES := -Icoders

SRCS := \
	coders/main.c \
	coders/args.c \
	coders/time.c \
	coders/log.c \
	coders/init.c \
	coders/coder.c \
	coders/dongle.c \
	coders/scheduler.c \
	coders/queue.c \
	coders/monitor.c \
	coders/utils.c \
	coders/sim.c \
	coders/sim_alloc.c

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

coders/%.o: coders/%.c coders/codexion.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re