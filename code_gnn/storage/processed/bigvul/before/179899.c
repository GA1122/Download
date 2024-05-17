 local void process(char *path)
 {
     int method = -1;                 
     size_t len;                      
     struct stat st;                  
      
     static char *sufs[] = {".z", "-z", "_z", ".Z", ".gz", "-gz", ".zz", "-zz",
                            ".zip", ".ZIP", ".tgz", NULL};
 
      
     if (path == NULL) {
         strcpy(g.inf, "<stdin>");
         g.ind = 0;
         g.name = NULL;
         g.mtime = g.headis & 2 ?
                   (fstat(g.ind, &st) ? time(NULL) : st.st_mtime) : 0;
         len = 0;
     }
     else {
          
         if (path != g.inf) {
             strncpy(g.inf, path, sizeof(g.inf));
             if (g.inf[sizeof(g.inf) - 1])
                 bail("name too long: ", path);
         }
         len = strlen(g.inf);
 
          
         if (lstat(g.inf, &st)) {
             if (errno == ENOENT && (g.list || g.decode)) {
                 char **try = sufs;
                 do {
                     if (*try == NULL || len + strlen(*try) >= sizeof(g.inf))
                         break;
                     strcpy(g.inf + len, *try++);
                     errno = 0;
                 } while (lstat(g.inf, &st) && errno == ENOENT);
             }
 #ifdef EOVERFLOW
             if (errno == EOVERFLOW || errno == EFBIG)
                 bail(g.inf,
                     " too large -- not compiled with large file support");
 #endif
             if (errno) {
                 g.inf[len] = 0;
                 complain("%s does not exist -- skipping", g.inf);
                 return;
             }
             len = strlen(g.inf);
         }
 
          
         if ((st.st_mode & S_IFMT) != S_IFREG &&
             (st.st_mode & S_IFMT) != S_IFLNK &&
             (st.st_mode & S_IFMT) != S_IFDIR) {
             complain("%s is a special file or device -- skipping", g.inf);
             return;
         }
         if ((st.st_mode & S_IFMT) == S_IFLNK && !g.force && !g.pipeout) {
             complain("%s is a symbolic link -- skipping", g.inf);
             return;
         }
         if ((st.st_mode & S_IFMT) == S_IFDIR && !g.recurse) {
             complain("%s is a directory -- skipping", g.inf);
             return;
         }
 
          
         if ((st.st_mode & S_IFMT) == S_IFDIR) {
             char *roll, *item, *cut, *base, *bigger;
             size_t len, hold;
             DIR *here;
             struct dirent *next;
 
              
             here = opendir(g.inf);
             if (here == NULL)
                 return;
             hold = 512;
             roll = MALLOC(hold);
             if (roll == NULL)
                 bail("not enough memory", "");
             *roll = 0;
             item = roll;
             while ((next = readdir(here)) != NULL) {
                 if (next->d_name[0] == 0 ||
                     (next->d_name[0] == '.' && (next->d_name[1] == 0 ||
                      (next->d_name[1] == '.' && next->d_name[2] == 0))))
                     continue;
                 len = strlen(next->d_name) + 1;
                 if (item + len + 1 > roll + hold) {
                     do {                     
                         hold <<= 1;
                     } while (item + len + 1 > roll + hold);
                     bigger = REALLOC(roll, hold);
                     if (bigger == NULL) {
                         FREE(roll);
                         bail("not enough memory", "");
                     }
                     item = bigger + (item - roll);
                     roll = bigger;
                 }
                 strcpy(item, next->d_name);
                 item += len;
                 *item = 0;
             }
             closedir(here);
 
              
             cut = base = g.inf + strlen(g.inf);
             if (base > g.inf && base[-1] != (unsigned char)'/') {
                 if ((size_t)(base - g.inf) >= sizeof(g.inf))
                     bail("path too long", g.inf);
                 *base++ = '/';
             }
             item = roll;
             while (*item) {
                 strncpy(base, item, sizeof(g.inf) - (base - g.inf));
                 if (g.inf[sizeof(g.inf) - 1]) {
                     strcpy(g.inf + (sizeof(g.inf) - 4), "...");
                     bail("path too long: ", g.inf);
                 }
                 process(g.inf);
                 item += strlen(item) + 1;
             }
             *cut = 0;
 
              
             FREE(roll);
             return;
         }
 
          
         if (!(g.force || g.list || g.decode) && len >= strlen(g.sufx) &&
                 strcmp(g.inf + len - strlen(g.sufx), g.sufx) == 0) {
             complain("%s ends with %s -- skipping", g.inf, g.sufx);
             return;
         }
 
          
         if (g.decode == 1 && !g.pipeout && !g.list) {
             int suf = compressed_suffix(g.inf);
             if (suf == 0) {
                 complain("%s does not have compressed suffix -- skipping",
                          g.inf);
                 return;
             }
             len -= suf;
         }
 
          
         g.ind = open(g.inf, O_RDONLY, 0);
         if (g.ind < 0)
             bail("read error on ", g.inf);
 
          
         g.name = g.headis & 1 ? justname(g.inf) : NULL;
         g.mtime = g.headis & 2 ? st.st_mtime : 0;
     }
     SET_BINARY_MODE(g.ind);
 
      
     g.hname = NULL;
     if (g.decode) {
         in_init();
         method = get_header(1);
         if (method != 8 && method != 257 &&
                  
                 !(method == -2 && g.force && g.pipeout && g.decode != 2 &&
                   !g.list)) {
             RELEASE(g.hname);
             if (g.ind != 0)
                 close(g.ind);
             if (method != -1)
                 complain(method < 0 ? "%s is not compressed -- skipping" :
                          "%s has unknown compression method -- skipping",
                          g.inf);
             return;
         }
 
          
         if (g.decode == 2) {
             if (method == 8)
                 infchk();
             else {
                 unlzw();
                 if (g.list) {
                     g.in_tot -= 3;
                     show_info(method, 0, g.out_tot, 0);
                 }
             }
             RELEASE(g.hname);
             if (g.ind != 0)
                 close(g.ind);
             return;
         }
     }
 
      
     if (g.list) {
         list_info();
         RELEASE(g.hname);
         if (g.ind != 0)
             close(g.ind);
         return;
     }
 
      
     if (path == NULL || g.pipeout) {
          
         g.outf = MALLOC(strlen("<stdout>") + 1);
         if (g.outf == NULL)
             bail("not enough memory", "");
         strcpy(g.outf, "<stdout>");
         g.outd = 1;
         if (!g.decode && !g.force && isatty(g.outd))
             bail("trying to write compressed data to a terminal",
                   " (use -f to force)");
      }
      else {
        char *to, *repl;
//         char *to = g.inf, *sufx = "";
//         size_t pre = 0;
// 
          
        to = g.inf;
        if (g.decode && (g.headis & 1) != 0 && g.hname != NULL) {
            to = g.hname;
            len = strlen(g.hname);
//         if (g.decode) {
//              
//             if ((g.headis & 1) != 0 && g.hname != NULL) {
//                 pre = justname(g.inf) - g.inf;
//                 to = justname(g.hname);
//                 len = strlen(to);
//             }
//              
//             else if (strcmp(to + len, ".tgz") == 0)
//                 sufx = ".tar";
          }
         
        repl = g.decode && strcmp(to + len, ".tgz") ? "" : ".tar";
//         else
//              
//             sufx = g.sufx;
  
           
        g.outf = MALLOC(len + (g.decode ? strlen(repl) : strlen(g.sufx)) + 1);
//         g.outf = MALLOC(pre + len + strlen(sufx) + 1);
          if (g.outf == NULL)
              bail("not enough memory", "");
        memcpy(g.outf, to, len);
        strcpy(g.outf + len, g.decode ? repl : g.sufx);
//         memcpy(g.outf, g.inf, pre);
//         memcpy(g.outf + pre, to, len);
//         strcpy(g.outf + pre + len, sufx);
          g.outd = open(g.outf, O_CREAT | O_TRUNC | O_WRONLY |
                             (g.force ? 0 : O_EXCL), 0600);
//                               (g.force ? 0 : O_EXCL), 0600);
  
           
          if (g.outd < 0 && errno == EEXIST && isatty(0) && g.verbosity) {
             int ch, reply;
 
             fprintf(stderr, "%s exists -- overwrite (y/n)? ", g.outf);
             fflush(stderr);
             reply = -1;
             do {
                 ch = getchar();
                 if (reply < 0 && ch != ' ' && ch != '\t')
                     reply = ch == 'y' || ch == 'Y' ? 1 : 0;
             } while (ch != EOF && ch != '\n' && ch != '\r');
             if (reply == 1)
                 g.outd = open(g.outf, O_CREAT | O_TRUNC | O_WRONLY,
                               0600);
         }
 
          
         if (g.outd < 0 && errno == EEXIST) {
             complain("%s exists -- skipping", g.outf);
             RELEASE(g.outf);
             RELEASE(g.hname);
             if (g.ind != 0)
                 close(g.ind);
             return;
         }
 
          
         if (g.outd < 0)
             bail("write error on ", g.outf);
     }
     SET_BINARY_MODE(g.outd);
     RELEASE(g.hname);
 
      
     if (g.verbosity > 1)
         fprintf(stderr, "%s to %s ", g.inf, g.outf);
     if (g.decode) {
         if (method == 8)
             infchk();
         else if (method == 257)
             unlzw();
         else
             cat();
     }
 #ifndef NOTHREAD
     else if (g.procs > 1)
         parallel_compress();
 #endif
     else
         single_compress(0);
     if (g.verbosity > 1) {
         putc('\n', stderr);
         fflush(stderr);
     }
 
      
     if (g.ind != 0)
         close(g.ind);
     if (g.outd != 1) {
         if (close(g.outd))
             bail("write error on ", g.outf);
         g.outd = -1;             
         if (g.ind != 0) {
             copymeta(g.inf, g.outf);
             if (!g.keep)
                 unlink(g.inf);
         }
         if (g.decode && (g.headis & 2) != 0 && g.stamp)
             touch(g.outf, g.stamp);
     }
     RELEASE(g.outf);
 }