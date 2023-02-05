NAME		= minishell
BONUS		= minishell_bonus

SRCS		= main.c\
			  builtins/env.c builtins/pwd.c builtins/export.c builtins/var_env.c\
			  builtins/unset.c builtins/utils.c builtins/print_args.c builtins/cd.c\
			  builtins/echo.c builtins/exit.c builtins/export_utils.c\
			  builtins/var_utils.c\
			  parsing/parsing.c \
			  parsing/parsing_utils.c parsing/split_token.c \
			  parsing/str_get_token.c parsing/stack_func.c\
			  parsing/quote_utils.c parsing/split_token_1.c\
			  parsing/split_utils.c parsing/quote_in_split.c \
			  execution/exec_cmd.c execution/execution.c execution/find_path.c \
			  execution/exec_utils.c execution/signals_handlers.c\
			  execution/exit.c execution/heredoc.c\
			  execution/builtins_execution.c execution/fd_manager.c\

SRCS_B		= bonus/main.c\
			  bonus/builtins/env.c bonus/builtins/pwd.c\
			  bonus/builtins/export.c bonus/builtins/var_env.c\
			  bonus/builtins/unset.c bonus/builtins/utils.c\
			  bonus/builtins/print_args.c bonus/builtins/cd.c\
			  bonus/builtins/echo.c bonus/builtins/exit.c bonus/builtins/export_utils.c\
			  bonus/builtins/var_utils.c\
			  bonus/parsing/parsing.c\
			  bonus/parsing/parsing_utils.c bonus/parsing/split_token.c \
			  bonus/parsing/str_get_token.c bonus/parsing/stack_func.c\
			  bonus/execution/exec_cmd.c bonus/execution/execution.c bonus/execution/find_path.c\
			  bonus/execution/exec_utils.c bonus/execution/signals_handlers.c\
			  bonus/execution/exit.c bonus/execution/heredoc.c\
			  bonus/execution/builtins_execution.c bonus/execution/fd_manager.c\

CC			= @clang
LIBS        = -Llibft -lft -lreadline
CFLAGS		= -Wall -Werror -Wextra -g -gdwarf-4
OBJS		= ${SRCS:.c=.o}
OBJS_B		= ${SRCS_B:.c=.o}

all: ${NAME}

.c.o :
	@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): ${OBJS}
	@make -C ./libft --no-print-directory
	${CC} ${CFLAGS} -o ${NAME} ${SRCS} ${LIBS}
	@printf '\x1b[38;2;255;0;0m███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓   \n▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░    \n▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒\n░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░   \n       ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\x1b[0m\n'

clean:
	@rm -f ${OBJS}
	@printf '\e[5m❌ \e[0m\x1b[38;2;255;140;0mObjects removed\x1b[0m\e[5m ❌\n\e[0m'

fclean : clean
	@rm -f ${NAME}
	@printf '\e[5m🚨 \e[0m\x1b[38;2;200;0;20mBinary removed\x1b[0m\e[5m 🚨\n\e[0m'

re: fclean all

bonus: ${BONUS}

${BONUS}: ${OBJS_B}
	@make -C ./libft > /dev/null
	@printf '\x1b[38;2;250;0;0mLibft compiled 🤓\n\x1b[0m'
	$(CC) ${OBJS_B} -o ${BONUS} $(LIBS)
	@printf '\x1b[38;2;50;205;50mMinishell bonus compiled 🤓\n\x1b[0m'

clean_b:
	@rm -f $(OBJS_B)
	@printf '\e[5m❌ \e[0m\x1b[38;2;255;140;0mBonus objects removed\x1b[0m\e[5m ❌\n\e[0m'

fclean_b: clean_b
	@rm -f ${BONUS}
	@printf '\e[5m🚨 \e[0m\x1b[38;2;200;0;20mBonus binary removed\x1b[0m\e[5m 🚨\n\e[0m'

re_b: fclean_b bonus

norm:
	norminette */*.c && norminette */*.h

vg:
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=.vsupp ./minishell

.PHONY: all clean fclean re bonus clean_b fclean_b re_b
