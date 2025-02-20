#include "puzzle_common.h"
#include "puzzle.h"

typedef struct Opts_ {
    const char *file1;
    int exit;
} Opts;

void usage(void)
{
    puts("\nUsage: puzzle-sign [-b <contrast barrier for cropping] [-c]\n"
         "   [-C <max cropping ratio>] [-e] [-E similarity threshold] [-h]\n"
         "   [-H <max height>] [-l <lambdas>] [-n <noise cutoff>]\n"
         "   [-p <p ratio>] [-t] [-W <max width>] <file 1>\n\n"
         "Compute signature\n\n"
         "-b <contrast barrier for cropping>\n"
         "-c : disable autocrop\n"
         "-C <max cropping ratio>\n"
         "-e : exit with 10 (images are similar) or 20 (images are not)\n"
         "-h : show help\n"
         "-H <width> : set max height\n"
         "-l <lambdas> : change lambdas\n"
         "-n <noise cutoff> : change noise cutoff\n"
         "-p <ratio> : set p ratio\n"
         "-t disable fix for texts\n"
         "-W <width> : set max width\n"
         "\n");
    exit(EXIT_SUCCESS);
}

int parse_opts(Opts * const opts, PuzzleContext * context,
               int argc, char * const *argv) {
    int opt;
    extern char *optarg;
    extern int optind;

    opts->exit = 0;
    while ((opt = getopt(argc, argv, "b:cC:eE:hH:l:n:p:tW:")) != -1) {
        switch (opt) {
        case 'b':
            puzzle_set_contrast_barrier_for_cropping(context, atof(optarg));
            break;
        case 'c':
            puzzle_set_autocrop(context, 0);
            break;            
        case 'C':
            puzzle_set_max_cropping_ratio(context, atof(optarg));
            break;
        case 'e':
            opts->exit = 1;
            break;
        case 'h':
            usage();
            /* NOTREACHED */
        case 'H':
            puzzle_set_max_height(context, strtoul(optarg, NULL, 10));
            break;
        case 'l':
            puzzle_set_lambdas(context, strtoul(optarg, NULL, 10));
            break;
        case 'n':
            puzzle_set_noise_cutoff(context, atof(optarg));
            break;
        case 'p':
            puzzle_set_p_ratio(context, atof(optarg));
            break;
        case 'W':
            puzzle_set_max_width(context, strtoul(optarg, NULL, 10));
            break;
        default:
            usage();
            /* NOTREACHED */            
        }
    }
    argc -= optind;
    argv += optind;
    if (argc != 1) {
        usage();
    }
    opts->file1 = *argv;
    
    return 0;
}

int main(int argc, char *argv[])
{
    Opts opts;
    PuzzleContext context;
    PuzzleCvec cvec1;
    double d;
    
    puzzle_init_context(&context);    
    parse_opts(&opts, &context, argc, argv);
    puzzle_init_cvec(&context, &cvec1);
    if (puzzle_fill_cvec_from_file(&context, &cvec1, opts.file1) != 0) {    
        //fprintf(stderr, "Unable to read [%s]\n", opts.file1);
        return 1;
    }

    puzzle_dump_cvec(&context, &cvec1);

    puzzle_free_cvec(&context, &cvec1);
    puzzle_free_context(&context);
    return 10;
}
