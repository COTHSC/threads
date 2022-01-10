NAME = philosophers 

SRCS = ./philosophers.c  \

OBJS = $(SRCS:.c=.o)

UNAME_S := $(shell uname -s)
   ifeq ($(UNAME_S),Linux)
        CC=clang
    endif
    ifeq ($(UNAME_S),Darwin)
        CC= clang
    endif

CFLAGS = -Wall -Wextra -Werror

GFLAG = -g3 -fsanitize=thread

INCLUDES= -I includes/

all : $(NAME)

$(NAME) : $(OBJS)
	$(MAKE) -C ./libft/
	$(CC) -pthreads $(CFLAGS) $(GFLAGS) -o $(NAME) $(OBJS) ./libft/libft.a

%.o: %.c
	$(CC) -o $@ -c $^ $(CFLAGS) $(GFLAGS) $(INCLUDES)

clean :
	rm -f $(OBJS)
	$(MAKE) -C ./libft/ clean

fclean:	clean
	rm -f $(NAME)
	$(MAKE) -C ./libft/ fclean

re: fclean all
