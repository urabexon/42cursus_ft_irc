# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: urabex <urabex@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/16 01:13:11 by urabex            #+#    #+#              #
#    Updated: 2025/05/16 01:16:06 by urabex           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -pedantic-errors
RM			=	rm -rf

INCS		=	-I includes/
SRCS_DIR	=	srcs/
OBJS_DIR	=	objs/
SRCS_FILES	=	main.cpp Server.cpp Client.cpp Channel.cpp manageServerLoop.cpp \
				handlePollFd.cpp parse.cpp utils.cpp commands/invite.cpp commands/join.cpp \
				commands/kick.cpp commands/mode.cpp commands/nick.cpp commands/part.cpp \
				commands/pass.cpp commands/privmsg.cpp commands/quit.cpp commands/topic.cpp \
				commands/user.cpp commands/cmmandUtils.cpp commands/modes/invite.cpp commands/modes/key.cpp \
				commands/modes/limit.cpp commands/modes/operator.cpp commands/modes/topic.cpp
SRCS		=	$(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS		=	$(patsubst $(SRCS_DIR)%.cpp, $(OBJS_DIR)%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) $(OBJS) -o $@

$(OBJS_DIR)%.o : $(SRCS_DIR)%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean:
	$(RM) $(OBJS_DIR) $(NAME)

re: fclean all

.PHONY: all clean fclean re