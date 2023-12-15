# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atudor <atudor@student.42barcelon>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/25 05:04:24 by atudor            #+#    #+#              #
#    Updated: 2023/12/12 14:57:44 by atudor           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= pipex
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
RM		= rm -f

SRC_DIR	= srcs/
FILES	= pipex pipex_utils

HEADER	= includes/
LIB_H	= libft/
LIBFT_DIR = libft

INCLUDE	= -I$(HEADER) -I$(LIB_H)

SRCS	= $(addprefix $(SRC_DIR), $(addsuffix .c,$(FILES)))
OBJS	= $(SRCS:.c=.o)
# DEPS	= $(subst .o,.d,$(OBJS))
LIB		= libft/

%.o: %.c $(HEADER) $(LIB_H) Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

#	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: libft.a ${OBJS}
	    $(CC) $(CFLAGS) -o ${NAME} ${OBJS} -L $(LIBFT_DIR) -lft
	    @echo "Pipex compiled!"

.SILENT:

libft.a:
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "Libft compiled!"

all:
	@$(MAKE) -C $(LIB)
	@$(MAKE) $(NAME)

#bonus:		all

clean:
	@${RM} $(OBJS) $(DEPS)
	@$(MAKE) -C $(LIB) clean 

fclean:		clean
			${RM} ${NAME}
			$(MAKE) -C $(LIB) fclean 

re:			fclean all

.PHONY:		all clean fclean re libft.a

