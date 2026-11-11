NAME		= ft_malcolm

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
CPPFLAGS	= -I$(LIBFT_DIR) -MMD -MP

MAKEFLAGS	+= --no-print-directory

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

OBJ_DIR		= obj

SRCS		= main.c
OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
