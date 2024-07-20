#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//#define RSH_RL_BUFSIZE 1024

#define RSH_TOK_BUFSIZE 64
#define RSH_TOK_DELIM " \t\r\n\a"

/*
  Function Declarations for builtin shell commands:
 */
int rsh_cd(char **args);
int rsh_help(char **args);
int rsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &rsh_cd,
  &rsh_help,
  &rsh_exit
};

int rsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int rsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "rsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("rsh");
        }
    }
    return 1;
}

int rsh_help(char **args) {
    int i;
    printf("Dimitri Kaiser's RSH, made after Stephen Brennan's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < rsh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int rsh_exit(char **args) {
    return 0;
}

/// @brief reads input into the buffer until reaching the EOF or a \n character.
/// @return pointer to the read line (DOES NOT FREE ALLOCATED MEMORY)
/*
char *rsh_read_line(void) {
    int bufsize = RSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "rsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += RSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "rsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
} */

/// @brief reads input into the buffer until reaching the EOF or a \n character.
/// @return pointer to the read line (DOES NOT FREE ALLOCATED MEMORY)
char *rsh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char **rsh_split_line(char *line) {
    int bufsize = RSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "rsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, RSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += RSH_TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);
            if (!tokens) {
                fprintf(stderr, "rsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, RSH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

int rsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("rsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("rsh");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int rsh_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        // an empty command was entered.
        return 1;
    }

    for (i = 0; i < rsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return rsh_launch(args);
}

void rsh_loop(void) {
    char *line;
    char **args;
    int status;

    int uid = getuid();
    char prompt = uid == 0 ? '#' : '$';

    do {
        printf("%c ", prompt);
        line = rsh_read_line();
        args = rsh_split_line(line);
        status = rsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv) {
    rsh_loop();

    return EXIT_SUCCESS;
}
