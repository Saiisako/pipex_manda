NAME = pipex
CC = cc
FLAGS = -Wall -Werror -Wextra -g
RM = rm -rf
LIBFT = library/libft/libft.a
LIBFT_FLAGS = -L./library/libft -lft

SRCS	=	pipex.c


OBJS_BONUS = $(SRCS_BONUS:.c=.o)
OBJS = $(SRCS:.c=.o)

TARGET = $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(MLX_FLAGS) $(LIBFT_FLAGS)

$(LIBFT):
	make -C library/libft

bonus : $(NAME_BONUS)

.c.o:
	$(CC) $(FLAGS) -c -o $@ $<

all: $(NAME)

clean:
	@$(RM) $(OBJS)
	@make -s -C library/libft clean
	@echo "files erased successfully"

fclean: clean
	@$(RM) $(NAME)
	@make -s -C library/libft fclean

re: fclean all

.PHONY: all clean fclean re bonus minilibx
