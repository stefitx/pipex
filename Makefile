# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/25 05:04:24 by atudor            #+#    #+#              #
#    Updated: 2024/01/02 17:33:20 by atudor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= pipex
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
RM		= rm -f

BONUS_SRCS_DIR = bonus/srcs/
BONUS_FILES = pipex_bonus pipex_utils_bonus pipex_process_bonus
SRC_DIR	= srcs/
FILES	= pipex pipex_utils pipex_process_utils

BONUS_HEADER = bonus/includes/
HEADER	= includes/
LIB_H	= libft/
LIBFT_DIR = libft

INCLUDE	= -I$(HEADER) -I$(LIB_H)
BONUS_INCLUDE = -I$(BONUS_HEADER) -I$(LIB_H)

BONUS_SRCS = $(addprefix $(BONUS_SRCS_DIR), $(addsuffix .c,$(BONUS_FILES)))
SRCS = $(if $(BONUS), $(addprefix $(BONUS_SRCS_DIR), $(addsuffix .c,$(BONUS_FILES))), $(addprefix $(SRC_DIR), $(addsuffix .c,$(FILES))))
BONUS_OBJS = $(BONUS_SRCS:.c=.o)
OBJS	= $(SRCS:.c=.o)
LIB		= libft/

%.o: %.c $(HEADER) $(LIB_H) Makefile
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "Compiling $<"

${NAME}: libft.a ${OBJS} 
	@$(CC) $(CFLAGS) -o ${NAME} ${OBJS} -L $(LIBFT_DIR) -lft
	@echo "Pipex compiled!"

${BONUS}: libft.a ${BONUS_OBJS}
	@$(CC) $(CFLAGS) -o ${NAME} ${BONUS_OBJS} -L $(LIBFT_DIR) -lft
	@echo "Bonus compiled!"

.SILENT:

libft.a:
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@echo "Libft compiled!"

all: 
	@$(MAKE) $(NAME) --no-print-directory

bonus:
	@$(MAKE) BONUS=1 $(NAME) --no-print-directory

clean:
	@${RM} $(OBJS) $(DEPS)
	@${RM} $(BONUS_OBJS) $(DEPS)
	@$(MAKE) -C $(LIB) clean --no-print-directory

fclean:		clean
			@${RM} ${NAME}
			@$(MAKE) -C $(LIB) fclean --no-print-directory

re:			fclean all

.PHONY:		all clean fclean re libft.a bonus

