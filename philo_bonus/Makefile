# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hcorrea- <hcorrea-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/23 11:49:36 by hcorrea-          #+#    #+#              #
#    Updated: 2023/03/30 10:37:57 by hcorrea-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN			=	\033[0;32m
RED				=	\033[0;31m
YELLOW			=	\033[0;33m
END				=	\033[0m

PHILO			=	philo

PROJECTDIR		=	src
INC				=	inc
OBJ_DIR			=	obj
HEADER			=	$(INC)

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -pthread -g -fsanitize=address

SRC				=	cleanup.c				\
					exit.c					\
					main_thread.c			\
					init.c					\
					ipc.c					\
					main.c					\
					output.c				\
					parsing.c				\
					philo_utils.c			\
					philo.c					\
					time.c					\
					utils.c					\

OBJ				=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

RM				=	rm -rf
MKDIR			=	mkdir -p

all:			 	$(PHILO)
					@echo "$(GREEN)Philo Bonus successfully compiled!$(END)"
					@sleep 0.5
					@clear
					
$(PHILO):			$(OBJ)
					@clear
					@echo "$(YELLOW)Compiling Philo Bonus...$(END)"
					@$(CC) $(CFLAGS) -I $(HEADER) $(OBJ) -o $@
					
$(OBJ_DIR)/%.o:		$(PROJECTDIR)/%.c $(HEADER)
					@$(MKDIR) $(dir $@)
					@$(CC) $(CFLAGS) -I $(HEADER) -o $@ -c $<

%.o:				%.c
					@$(CC) $(CFLAGS) -I $(HEADER) -c $< -o $@
					
clean:
					@$(RM) $(OBJ_DIR)
					@clear
					@echo "$(RED)All objects deleted!$(END)"

fclean:				clean
					@$(RM) $(PHILO)
					@echo "$(RED)All executables deleted!$(END)"
					@sleep 0.5
					@clear
					
re:					fclean all

.PHONY:	clean, fclean, re