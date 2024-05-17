local void defaults(void)
{
    g.level = Z_DEFAULT_COMPRESSION;
     
    ZopfliInitOptions(&g.zopts);
#ifdef NOTHREAD
    g.procs = 1;
#else
    g.procs = nprocs(8);
#endif
    g.block = 131072UL;              
    g.rsync = 0;                     
    g.setdict = 1;                   
    g.verbosity = 1;                 
    g.headis = 3;                    
    g.pipeout = 0;                   
    g.sufx = ".gz";                  
    g.decode = 0;                    
    g.list = 0;                      
    g.keep = 0;                      
    g.force = 0;                     
    g.recurse = 0;                   
    g.form = 0;                      
}
