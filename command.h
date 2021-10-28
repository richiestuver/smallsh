// contains definition for command struct used throughout shell execution

struct command {
    char* cmd;
    int argc;
    char** argv;
    char* f_stdin;
    char* f_stdout;
    char* f_stderr;
    bool background;
};