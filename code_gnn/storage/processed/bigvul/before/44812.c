int main(int argc, char **argv)
{
    int n;                           
    int noop;                        
    unsigned long done;              
    char *opts, *p;                  

     
    g.outf = NULL;
    g.first = 1;
    g.hname = NULL;

     
    p = strrchr(argv[0], '/');
    p = p == NULL ? argv[0] : p + 1;
    g.prog = *p ? p : "pigz";

     
    signal(SIGINT, cut_short);
#ifndef NOTHREAD
    yarn_prefix = g.prog;            
    yarn_abort = cut_short;          
#endif
#ifdef DEBUG
    gettimeofday(&start, NULL);      
    log_init();                      
#endif

     
    defaults();

     
    opts = getenv("GZIP");
    if (opts != NULL) {
        while (*opts) {
            while (*opts == ' ' || *opts == '\t')
                opts++;
            p = opts;
            while (*p && *p != ' ' && *p != '\t')
                p++;
            n = *p;
            *p = 0;
            if (option(opts))
                bail("cannot provide files in GZIP environment variable", "");
            opts = p + (n ? 1 : 0);
        }
        option(NULL);
    }

     
    opts = getenv("PIGZ");
    if (opts != NULL) {
        while (*opts) {
            while (*opts == ' ' || *opts == '\t')
                opts++;
            p = opts;
            while (*p && *p != ' ' && *p != '\t')
                p++;
            n = *p;
            *p = 0;
            if (option(opts))
                bail("cannot provide files in PIGZ environment variable", "");
            opts = p + (n ? 1 : 0);
        }
        option(NULL);
    }

     
    if (strcmp(g.prog, "unpigz") == 0 || strcmp(g.prog, "gunzip") == 0) {
        if (!g.decode)
            g.headis >>= 2;
        g.decode = 1;
    }
    if ((n = strlen(g.prog)) > 2 && strcmp(g.prog + n - 3, "cat") == 0) {
        if (!g.decode)
            g.headis >>= 2;
        g.decode = 1;
        g.pipeout = 1;
    }

     
    if (argc < 2 && isatty(g.decode ? 0 : 1))
        help();

     
    done = noop = 0;
    for (n = 1; n < argc; n++)
        if (noop == 0 && strcmp(argv[n], "--") == 0) {
            noop = 1;
            option(NULL);
        }
        else if (noop || option(argv[n])) {  
            if (done == 1 && g.pipeout && !g.decode && !g.list && g.form > 1)
                complain("warning: output will be concatenated zip files -- "
                         "will not be able to extract");
            process(strcmp(argv[n], "-") ? argv[n] : NULL);
            done++;
        }
    option(NULL);

     
    if (done == 0)
        process(NULL);

     
    new_opts();
    log_dump();
    return 0;
}
