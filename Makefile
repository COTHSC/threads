NAME = philosophers 

SRCS = ./srcs/main.c\
	   ./srcs/birth.c\
	   ./srcs/routine.c\
	   ./srcs/eat.c\
	   ./srcs/print.c\
	   ./srcs/timing_functions.c\
	   ./srcs/init_philos.c\
	   ./srcs/death.c\
	   ./srcs/args_check.c\
	   ./srcs/alloc_and_free.c\
	   ./not_libft/ft_atoi.c\
	   ./not_libft/ft_itoa.c\
	   ./not_libft/ft_isdigit.c\
	   ./not_libft/ft_calloc.c\
	   ./not_libft/ft_putstr_fd.c

OBJS = $(SRCS:.c=.o)

UNAME_S := $(shell uname -s)
   ifeq ($(UNAME_S),Linux)
        CC=clang
    endif
    ifeq ($(UNAME_S),Darwin)
        CC= clang
    endif

CFLAGS = -Wall -Wextra -Werror

INCLUDES= -I includes/

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -pthreads $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) -o $@ -c $^ $(CFLAGS) $(INCLUDES)

clean :
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re: fclean all
