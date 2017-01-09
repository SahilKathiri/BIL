#include "inc/mpc.h"

#include <stdio.h>
#include <editline/readline.h>
#include <editline/history.h>

#define STYLE_BOLD         "\033[1m"
#define STYLE_UNDERLINE    "\033[4m"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char tape[33000];
char* ip;
char inp;
int index;

mpc_parser_t* Symbol;
mpc_parser_t* Comment;
mpc_parser_t* Loop;
mpc_parser_t* Expr;
mpc_parser_t* Bf;
mpc_parser_t* Number;
mpc_parser_t* Command;
mpc_parser_t* CommandName;
mpc_parser_t* InvalidCommandArg;

void eval_symbol(char* a) {
	// char* a = t->contents;
	if(strcmp(a, "+") == 0) { ++*ip; }
	if(strcmp(a, "-") == 0) { --*ip; }
	if(strcmp(a, ">") == 0) { ip++; }
	if(strcmp(a, "<") == 0) { ip--; }
	if(strcmp(a, ".") == 0) { printf("%c", *ip); }
	if(strcmp(a, ",") == 0) { 
		scanf("%c", &inp);
		*ip = inp; 
		if (*ip == 0 || *ip == 10) {
			scanf("%c", &inp);
			*ip = inp;
		}
	}
}

void eval_loop(mpc_ast_t* t);
void eval_loop(mpc_ast_t* t) {
	for (int i = 0;; ++i) {
		if (strstr(t->children[i]->tag, "loop")) {
			eval_loop(t->children[i]);
		}
		else if (strstr(t->children[i]->tag, "expr")) {
			if (t->children[i]->children_num == 0) /* Single char expression*/
				eval_symbol(t->children[i]->contents);

			else
				for (int j = 0; j < t->children[i]->children_num; ++j) {
					eval_symbol(t->children[i]->children[j]->contents);
				}
		}
		else if (strcmp(t->children[i]->contents, "[") == 0) { 
			if (*ip == 0)
				break;
		}
		else if (strcmp(t->children[i]->contents, "]") == 0) { 
			i = -1; 
		}
	}
	// mpc_ast_print(t);
}

void print_tape(int index);
void command_print(char* index) {
	errno = 0;
	long i = strtol(index, NULL, 10);
	errno != ERANGE ? print_tape(i) : printf(STYLE_BOLD ANSI_COLOR_RED "Invalid Number\n" ANSI_COLOR_RESET);
}

void command_help() {
	printf("HELPtext goes here\n\n\n\nsadsadas\n\n\n\n");
}

void eval_command(mpc_ast_t* t) {
	// mpc_ast_print(t);

	char* arg;
	char* command_name;

	for (int i = 0; i < t->children_num; i++) {
		if (strstr(t->children[i]->tag, "inval_command_arg")) {
			printf(STYLE_BOLD ANSI_COLOR_RED "Invalid Arguments\n" ANSI_COLOR_RESET);
			return;
		}
	}

	if (t->children_num == 0) {
		command_name = t->contents;
		arg = "ip";
	}
	else if (t->children_num == 2) {
		command_name = t->children[0]->contents;
		arg = t->children[1]->contents;		
	}
	else {
		printf(STYLE_BOLD ANSI_COLOR_RED "Error: Too many arguments\n" ANSI_COLOR_RESET);
	}

	if (strcmp(command_name, "%%help") == 0) { command_help(); }
	if (strcmp(command_name, "%%print") == 0) { command_print(arg); }


}

void read_bf(mpc_ast_t* t) {
	if (strstr(t->tag, "symbol")) { 
		return eval_symbol(t->contents); 
	}
	if (strstr(t->tag, "loop")) {
		return eval_loop(t);
	}

	if (strstr(t->tag, "command")) {
		return eval_command(t);
	}
	
	for (int i = 0; i < t->children_num; ++i) {
		if (strcmp(t->children[i]->contents, "[") == 0) { continue; }
		if (strcmp(t->children[i]->contents, "]") == 0) { continue; }
		if(strcmp(t->children[i]->tag, "regex") == 0) { continue; }
		if(strstr(t->children[i]->tag, "comment")) { continue; }

		read_bf(t->children[i]);

	}
}

void print_tape(int index) {
	int start = (index/10) * 10;
	int end = start + (10 - 1);
	int _index = index % 10;

	printf("\n----------------------------------------------------------------------------------\n");

	printf("\n" STYLE_BOLD "Start" ANSI_COLOR_RESET " = %d\n", start);
	printf(STYLE_BOLD "End" ANSI_COLOR_RESET "   = %d\n", end);
	printf(STYLE_BOLD "Index" ANSI_COLOR_RESET " = %d\n", index);

	printf("\n+");
	for (int i = start; i <= end; ++i) {
		printf("-----+");
	}
	printf("\n|");
	for (int i = start; i <= end; ++i) {
		if ((i-start) == (ip - tape))
			printf(STYLE_BOLD ANSI_COLOR_GREEN " %03d" ANSI_COLOR_RESET " |", tape[i]);
		else
			printf(" %05d |", tape[i]);
	}
	printf("\n+");
	for (int i = start; i <= end; ++i) {
		printf("-----+");
	}
	printf("\n");
	for (int i = 0; i < _index; ++i) {
		printf("      ");
	}
	printf(STYLE_BOLD ANSI_COLOR_BLUE " ^" ANSI_COLOR_RESET);
	printf("\n");
	for (int i = 0; i < _index; ++i) {
		printf("      ");
	}
	printf(STYLE_BOLD ANSI_COLOR_BLUE " %3d\n\n" ANSI_COLOR_RESET, index);
}

int main(int argc, char** argv) {

    Symbol = mpc_new("symbol");
    Comment = mpc_new("comment");
    Loop = mpc_new("loop");
    Number = mpc_new("number");
    InvalidCommandArg = mpc_new("inval_command_arg");
    CommandName = mpc_new("command_name");
    Command = mpc_new("command");
    Expr   = mpc_new("expr");
    Bf = mpc_new("bf");

    mpca_lang(MPCA_LANG_DEFAULT,
    "                                 								\
        symbol  :  '+' | '-' | '.' | ',' | '<' | '>' | '+' ;       	\
        comment  :  /[^\\+\\-\\.,\\[\\]><%%]/ ;   					\
        loop  :  '[' <expr>* ']' ;   								\
        number  :  /[0-9ip]+/ ;   									\
        inval_command_arg  :  /[^0-9ip]+/ ;   							\
        command_name  :  /%%[a-z]+/ ;   							\
        command  :  <command_name> <number> | <command_name> <inval_command_arg> | <command_name> ;		\
        expr  :  <loop> | <symbol>+ | <comment> ; 		 			\
        bf  : /^/ <expr>* /$/  | <command> ;       					\
    ",
    Symbol, Comment, Loop, Number, InvalidCommandArg, CommandName, Command, Expr, Bf);

    ip = tape;

    if (argc == 1) {
	    puts(STYLE_BOLD ANSI_COLOR_BLUE "Fck It! Version 0.0.0.1.0" ANSI_COLOR_RESET);
	    puts("Press " STYLE_BOLD ANSI_COLOR_RED "Ctrl+c" ANSI_COLOR_RESET " to Exit\n");
	    while (1) {

	        char* input = readline("fck> ");
	        add_history(input);

	        mpc_result_t r;
	        if (mpc_parse("<stdin>", input, Bf, &r)) {
	            read_bf(r.output);
	            // putchar('\n');
	            // print_tape();
	            // mpc_ast_print(r.output);
	            mpc_ast_delete(r.output);
	        } else {    
	            mpc_err_print(r.error);
	            mpc_err_delete(r.error);
	        }

	        free(input);

	    }
    }

    if (argc >= 2) {
    	for (int i = 1; i < argc; ++i) {
    		mpc_result_t r;
    		if (mpc_parse_contents(argv[i], Bf, &r)) {
    			read_bf(r.output);
				putchar('\n');
	            print_tape(ip - tape);
	            mpc_ast_delete(r.output);
    		} else {    
	            mpc_err_print(r.error);
	            mpc_err_delete(r.error);
	        }
    	}
    }

    mpc_cleanup(8, Symbol, Comment, Loop, Number, InvalidCommandArg, CommandName, Command, Expr, Bf);

    return 0;
}