#include <poke.h>

#include <stdio.h>
#include <stdlib.h>

#include <argp.h>
#include <wordexp.h>

//#define SSH_PATH "/home/spacekookie/.ssh/config"

int poke_connect(pk_parse_hst *host);
int poke_genkey(pk_parse_hst *host);

/* Declare const strings with metadata */
const char *argp_program_version = "Poke 0.6b";
const char *argp_program_bug_address = "<kookie@spacekookie.de>";

static char doc[] = "Poke -- a powerful ssh utility";
static char args_doc[] = "SERVER";

static struct argp_option options[] = { {"generate",  'g', 0,      0,  "Generate a new key for the provided server" } };

struct arguments {
    char *server;
    int generate;
};

/** This function parses arguments into our carrier struct  */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
        case 'g':
            arguments->generate = 1;
            break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 2) argp_usage(state);
            arguments->server = arg;
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1) argp_usage(state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Define the parser struct with all data */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv)
{
    /* Start by parsing arguments */
    struct arguments arguments = { 0, 0 };
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    /* Print out the options and stuff */
    printf ("Server = %s\nGenerate = %s\n", arguments.server, arguments.generate ? "yes" : "no");

    wordexp_t exp_result;
    wordexp("~/.ssh/config", &exp_result, 0);
    printf("SSH Path: %s\n", exp_result.we_wordv[0]);

    /* Create a context for a test path */
    pk_parse_ctx parser;

    /* Parse the ssh config */
    pk_parse_init(&parser, exp_result.we_wordv[0]);
    pk_parse_load(&parser);

    pk_parse_hst *host;
    int err = pk_parse_query(&parser, &host, arguments.server);
    if(err) {
        printf("Server %s not found in config %s!\n", arguments.server, exp_result.we_wordv[0]);
        return 255;
    }

    if(arguments.generate) {
        poke_genkey(host);
    } else {
        poke_connect(host);
    }

    pk_parse_free(&parser);
    exit (0);
}

int poke_connect(pk_parse_hst *host)
{
    char cmd[256];
    memset(cmd, 0, 256);
    sprintf(cmd, "ssh %s", host->host_id);

    system(cmd);
    return 0;
}

int poke_genkey(pk_parse_hst *host)
{
    char cmd[256];
    memset(cmd, 0, 256);
    sprintf(cmd, "echo -e  'y\\n' | ssh-keygen -t ed25519 -f ~/.ssh/%s_auto -P ''", host->host_id);

    /* Execute command */
    system(cmd);
    return 0;
}