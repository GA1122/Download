 context_length_arg (char const *str, int *out)
// context_length_arg (char const *str, intmax_t *out)
  {
  uintmax_t value;
  if (! (xstrtoumax (str, 0, 10, &value, "") == LONGINT_OK
         && 0 <= (*out = value)
         && *out == value))
//   switch (xstrtoimax (str, 0, 10, out, ""))
      {
//     case LONGINT_OK:
//     case LONGINT_OVERFLOW:
//       if (0 <= *out)
//         break;
//        
//     default:
        error (EXIT_TROUBLE, 0, "%s: %s", str,
               _("invalid context length argument"));
      }
    page size, unless a read yields a partial page.  */
 
 static char *buffer;		 
 static size_t bufalloc;		 
 #define INITIAL_BUFSIZE 32768	 
 static int bufdesc;		 
 static char *bufbeg;		 
 static char *buflim;		 
 static size_t pagesize;		 
 static off_t bufoffset;		 
 static off_t after_last_match;	 
 
  
 #define ALIGN_TO(val, alignment) \
   ((size_t) (val) % (alignment) == 0 \
    ? (val) \
    : (val) + ((alignment) - (size_t) (val) % (alignment)))
 
  
 static int
 reset (int fd, char const *file, struct stats *stats)
 {
   if (! pagesize)
     {
       pagesize = getpagesize ();
       if (pagesize == 0 || 2 * pagesize + 1 <= pagesize)
         abort ();
       bufalloc = ALIGN_TO (INITIAL_BUFSIZE, pagesize) + pagesize + 1;
       buffer = xmalloc (bufalloc);
     }
 
   bufbeg = buflim = ALIGN_TO (buffer + 1, pagesize);
   bufbeg[-1] = eolbyte;
   bufdesc = fd;
 
   if (S_ISREG (stats->stat.st_mode))
     {
       if (file)
         bufoffset = 0;
       else
         {
           bufoffset = lseek (fd, 0, SEEK_CUR);
           if (bufoffset < 0)
             {
               suppressible_error (_("lseek failed"), errno);
               return 0;
             }
         }
     }
   return 1;
 }
 
  
 static int
 fillbuf (size_t save, struct stats const *stats)
 {
   size_t fillsize = 0;
   int cc = 1;
   char *readbuf;
   size_t readsize;
 
    
   size_t saved_offset = buflim - save - buffer;
 
   if (pagesize <= buffer + bufalloc - buflim)
     {
       readbuf = buflim;
       bufbeg = buflim - save;
     }
   else
     {
       size_t minsize = save + pagesize;
       size_t newsize;
       size_t newalloc;
       char *newbuf;
 
        
       for (newsize = bufalloc - pagesize - 1; newsize < minsize; newsize *= 2)
         if (newsize * 2 < newsize || newsize * 2 + pagesize + 1 < newsize * 2)
           xalloc_die ();
 
        
       if (S_ISREG (stats->stat.st_mode))
         {
           off_t to_be_read = stats->stat.st_size - bufoffset;
           off_t maxsize_off = save + to_be_read;
           if (0 <= to_be_read && to_be_read <= maxsize_off
               && maxsize_off == (size_t) maxsize_off
               && minsize <= (size_t) maxsize_off
               && (size_t) maxsize_off < newsize)
             newsize = maxsize_off;
         }
 
        
       newalloc = newsize + pagesize + 1;
 
       newbuf = bufalloc < newalloc ? xmalloc (bufalloc = newalloc) : buffer;
       readbuf = ALIGN_TO (newbuf + 1 + save, pagesize);
       bufbeg = readbuf - save;
       memmove (bufbeg, buffer + saved_offset, save);
       bufbeg[-1] = eolbyte;
       if (newbuf != buffer)
         {
           free (buffer);
           buffer = newbuf;
         }
     }
 
   readsize = buffer + bufalloc - readbuf;
   readsize -= readsize % pagesize;
 
   if (! fillsize)
     {
       ssize_t bytesread;
       while ((bytesread = read (bufdesc, readbuf, readsize)) < 0
              && errno == EINTR)
         continue;
       if (bytesread < 0)
         cc = 0;
       else
         fillsize = bytesread;
     }
 
   bufoffset += fillsize;
 #if defined HAVE_DOS_FILE_CONTENTS
   if (fillsize)
     fillsize = undossify_input (readbuf, fillsize);
 #endif
   buflim = readbuf + fillsize;
   return cc;
 }
 
  
 static enum
 {
   BINARY_BINARY_FILES,
   TEXT_BINARY_FILES,
   WITHOUT_MATCH_BINARY_FILES
 } binary_files;		 
 
 static int filename_mask;	 
 static int out_quiet;		 
 static int out_invert;		 
 static int out_file;		 
 static int out_line;		 
 static int out_byte;		 
 static int out_before;		 
 static int out_after;		 
  static int out_file;		 
  static int out_line;		 
  static int out_byte;		 
static int out_before;		 
static int out_after;		 
// static intmax_t out_before;	 
// static intmax_t out_after;	 
  static int count_matches;	 
  static int list_files;		 
  static int no_filenames;	 
static off_t max_count;		 
// static intmax_t max_count;	 
  static int line_buffered;        
 static char const *lastnl;	 
 static char const *lastout;	 
 static uintmax_t totalnl;	 
 static off_t outleft;		 
 static int pending;		 
  static uintmax_t totalnl;	 
static off_t outleft;		 
static int pending;		 
// static intmax_t outleft;	 
// static intmax_t pending;	 
  static int done_on_match;	 
  static int exit_on_match;	 
  
 static uintmax_t
 add_count (uintmax_t a, uintmax_t b)
 {
   uintmax_t sum = a + b;
   if (sum < a)
     error (EXIT_TROUBLE, 0, _("input is too large to count"));
   return sum;
 }
 
 static void
 nlscan (char const *lim)
 {
   size_t newlines = 0;
   char const *beg;
   for (beg = lastnl; beg < lim; beg++)
     {
       beg = memchr (beg, eolbyte, lim - beg);
       if (!beg)
         break;
       newlines++;
     }
   totalnl = add_count (totalnl, newlines);
   lastnl = lim;
 }
 
  
 static void
 print_filename (void)
 {
   pr_sgr_start_if (filename_color);
   fputs (filename, stdout);
   pr_sgr_end_if (filename_color);
 }
 
  
 static void
 print_sep (char sep)
 {
   pr_sgr_start_if (sep_color);
   fputc (sep, stdout);
   pr_sgr_end_if (sep_color);
 }
 
  
 static void
 print_offset (uintmax_t pos, int min_width, const char *color)
 {
    
 
   char buf[sizeof pos * CHAR_BIT];
   char *p = buf + sizeof buf;
 
   do
     {
       *--p = '0' + pos % 10;
       --min_width;
     }
   while ((pos /= 10) != 0);
 
    
   if (align_tabs)
     while (--min_width >= 0)
       *--p = ' ';
 
   pr_sgr_start_if (color);
   fwrite (p, 1, buf + sizeof buf - p, stdout);
   pr_sgr_end_if (color);
 }
 
  
 static void
 print_line_head (char const *beg, char const *lim, int sep)
 {
   int pending_sep = 0;
 
   if (out_file)
     {
       print_filename ();
       if (filename_mask)
         pending_sep = 1;
       else
         fputc (0, stdout);
     }
 
   if (out_line)
     {
       if (lastnl < lim)
         {
           nlscan (beg);
           totalnl = add_count (totalnl, 1);
           lastnl = lim;
         }
       if (pending_sep)
         print_sep (sep);
       print_offset (totalnl, 4, line_num_color);
       pending_sep = 1;
     }
 
   if (out_byte)
     {
       uintmax_t pos = add_count (totalcc, beg - bufbeg);
 #if defined HAVE_DOS_FILE_CONTENTS
       pos = dossified_pos (pos);
 #endif
       if (pending_sep)
         print_sep (sep);
       print_offset (pos, 6, byte_num_color);
       pending_sep = 1;
     }
 
   if (pending_sep)
     {
        
       if (align_tabs)
         fputs ("\t\b", stdout);
 
       print_sep (sep);
     }
 }
 
 static const char *
 print_line_middle (const char *beg, const char *lim,
                    const char *line_color, const char *match_color)
 {
   size_t match_size;
   size_t match_offset;
   const char *cur = beg;
   const char *mid = NULL;
 
   while (cur < lim
          && ((match_offset = execute (beg, lim - beg, &match_size,
                                       beg + (cur - beg))) != (size_t) -1))
     {
       char const *b = beg + match_offset;
 
        
       if (b == lim)
         break;
 
        
       if (match_size == 0)
         {
            
            
           match_size = 1;
           if (!mid)
             mid = cur;
         }
       else
         {
            
           if (only_matching)
             print_line_head (b, lim, (out_invert ? SEP_CHAR_REJECTED
                                       : SEP_CHAR_SELECTED));
           else
             {
               pr_sgr_start (line_color);
               if (mid)
                 {
                   cur = mid;
                   mid = NULL;
                 }
               fwrite (cur, sizeof (char), b - cur, stdout);
             }
 
           pr_sgr_start_if (match_color);
           fwrite (b, sizeof (char), match_size, stdout);
           pr_sgr_end_if (match_color);
           if (only_matching)
             fputs ("\n", stdout);
         }
       cur = b + match_size;
     }
 
   if (only_matching)
     cur = lim;
   else if (mid)
     cur = mid;
 
   return cur;
 }
 
 static const char *
 print_line_tail (const char *beg, const char *lim, const char *line_color)
 {
   size_t eol_size;
   size_t tail_size;
 
   eol_size   = (lim > beg && lim[-1] == eolbyte);
   eol_size  += (lim - eol_size > beg && lim[-(1 + eol_size)] == '\r');
   tail_size  =  lim - eol_size - beg;
 
   if (tail_size > 0)
     {
       pr_sgr_start (line_color);
       fwrite (beg, 1, tail_size, stdout);
       beg += tail_size;
       pr_sgr_end (line_color);
     }
 
   return beg;
 }
 
 static void
 prline (char const *beg, char const *lim, int sep)
 {
   int matching;
   const char *line_color;
   const char *match_color;
 
   if (!only_matching)
     print_line_head (beg, lim, sep);
 
   matching = (sep == SEP_CHAR_SELECTED) ^ !!out_invert;
 
   if (color_option)
     {
       line_color = (((sep == SEP_CHAR_SELECTED)
                      ^ (out_invert && (color_option < 0)))
                     ? selected_line_color  : context_line_color);
       match_color = (sep == SEP_CHAR_SELECTED
                      ? selected_match_color : context_match_color);
     }
   else
     line_color = match_color = NULL;  
 
   if ((only_matching && matching)
       || (color_option  && (*line_color || *match_color)))
     {
        
       if (matching && (only_matching || *match_color))
         beg = print_line_middle (beg, lim, line_color, match_color);
 
        
       if (!only_matching && *line_color)
         beg = print_line_tail (beg, lim, line_color);
     }
 
   if (!only_matching && lim > beg)
     fwrite (beg, 1, lim - beg, stdout);
 
   if (ferror (stdout))
     {
       write_error_seen = 1;
       error (EXIT_TROUBLE, 0, _("write error"));
     }
 
   lastout = lim;
 
   if (line_buffered)
     fflush (stdout);
 }
 
  
 static void
 prpending (char const *lim)
 {
   if (!lastout)
     lastout = bufbeg;
   while (pending > 0 && lastout < lim)
     {
       char const *nl = memchr (lastout, eolbyte, lim - lastout);
       size_t match_size;
       --pending;
       if (outleft
           || ((execute (lastout, nl + 1 - lastout,
                         &match_size, NULL) == (size_t) -1)
               == !out_invert))
         prline (lastout, nl + 1, SEP_CHAR_REJECTED);
       else
         pending = 0;
     }
 }
 
  
 static void
 prtext (char const *beg, char const *lim, int *nlinesp)
 {
   
  static void
prtext (char const *beg, char const *lim, int *nlinesp)
// prtext (char const *beg, char const *lim, intmax_t *nlinesp)
  {
    static int used;	 
    char const *bp, *p;
    char eol = eolbyte;
  int i, n;
//   intmax_t i, n;
  
    if (!out_quiet && pending > 0)
      prpending (beg);
        
 
       bp = lastout ? lastout : bufbeg;
       for (i = 0; i < out_before; ++i)
         if (p > bp)
           do
             --p;
           while (p[-1] != eol);
 
        
       if ((out_before || out_after) && used && p != lastout && group_separator)
         {
           pr_sgr_start_if (sep_color);
           fputs (group_separator, stdout);
           pr_sgr_end_if (sep_color);
           fputc ('\n', stdout);
         }
 
       while (p < beg)
         {
           char const *nl = memchr (p, eol, beg - p);
           nl++;
           prline (p, nl, SEP_CHAR_REJECTED);
           p = nl;
         }
     }
 
   if (nlinesp)
     {
        
       for (n = 0; p < lim && n < outleft; n++)
         {
           char const *nl = memchr (p, eol, lim - p);
           nl++;
           if (!out_quiet)
             prline (p, nl, SEP_CHAR_SELECTED);
           p = nl;
         }
       *nlinesp = n;
 
        
       after_last_match = bufoffset - (buflim - p);
     }
   else if (!out_quiet)
     prline (beg, lim, SEP_CHAR_SELECTED);
 
   pending = out_quiet ? 0 : out_after;
   used = 1;
 }
 
 static size_t
 do_execute (char const *buf, size_t size, size_t *match_size, char const *start_ptr)
 {
   size_t result;
   const char *line_next;
 
    
   if (MB_CUR_MAX == 1 || !match_icase)
     return execute (buf, size, match_size, start_ptr);
 
   for (line_next = buf; line_next < buf + size; )
     {
       const char *line_buf = line_next;
       const char *line_end = memchr (line_buf, eolbyte, (buf + size) - line_buf);
       if (line_end == NULL)
         line_next = line_end = buf + size;
       else
         line_next = line_end + 1;
 
       if (start_ptr && start_ptr >= line_end)
         continue;
 
       result = execute (line_buf, line_next - line_buf, match_size, start_ptr);
       if (result != (size_t) -1)
         return (line_buf - buf) + result;
     }
 
   return (size_t) -1;
 }
 
  
 static int
 grepbuf (char const *beg, char const *lim)
   
static int
// static intmax_t
  grepbuf (char const *beg, char const *lim)
  {
  int nlines, n;
//   intmax_t nlines, n;
    char const *p;
    size_t match_offset;
    size_t match_size;
     {
       char const *b = p + match_offset;
       char const *endp = b + match_size;
        
       if (b == lim)
         break;
       if (!out_invert)
         {
           prtext (b, endp, (int *) 0);
           nlines++;
          break;
        if (!out_invert)
          {
          prtext (b, endp, (int *) 0);
//           prtext (b, endp, NULL);
            nlines++;
            outleft--;
            if (!outleft || done_on_match)
             }
         }
       else if (p < b)
         {
           prtext (p, b, &n);
           nlines += n;
           outleft -= n;
           if (!outleft)
             return nlines;
         }
       p = endp;
     }
   if (out_invert && p < lim)
     {
       prtext (p, lim, &n);
       nlines += n;
       outleft -= n;
     }
   return nlines;
 }
 
  
 static int
 grep (int fd, char const *file, struct stats *stats)
   
static int
// static intmax_t
  grep (int fd, char const *file, struct stats *stats)
  {
  int nlines, i;
//   intmax_t nlines, i;
    int not_text;
    size_t residue, save;
    char oldc;
     return 0;
 
   if (file && directories == RECURSE_DIRECTORIES
       && S_ISDIR (stats->stat.st_mode))
     {
        
       if (close (fd) != 0)
         suppressible_error (file, errno);
       return grepdir (file, stats) - 2;
     }
 
   totalcc = 0;
   lastout = 0;
   totalnl = 0;
   outleft = max_count;
   after_last_match = 0;
   pending = 0;
 
   nlines = 0;
   residue = 0;
   save = 0;
 
   if (! fillbuf (save, stats))
     {
       suppressible_error (filename, errno);
       return 0;
     }
 
   not_text = (((binary_files == BINARY_BINARY_FILES && !out_quiet)
                || binary_files == WITHOUT_MATCH_BINARY_FILES)
               && memchr (bufbeg, eol ? '\0' : '\200', buflim - bufbeg));
   if (not_text && binary_files == WITHOUT_MATCH_BINARY_FILES)
     return 0;
   done_on_match += not_text;
   out_quiet += not_text;
 
   for (;;)
     {
       lastnl = bufbeg;
       if (lastout)
         lastout = bufbeg;
 
       beg = bufbeg + save;
 
        
       if (beg == buflim)
         break;
 
        
       oldc = beg[-1];
       beg[-1] = eol;
       for (lim = buflim; lim[-1] != eol; lim--)
         continue;
       beg[-1] = oldc;
       if (lim == beg)
         lim = beg - residue;
       beg -= residue;
       residue = buflim - lim;
 
       if (beg < lim)
         {
           if (outleft)
             nlines += grepbuf (beg, lim);
           if (pending)
             prpending (lim);
           if ((!outleft && !pending) || (nlines && done_on_match && !out_invert))
             goto finish_grep;
         }
 
        
       i = 0;
       beg = lim;
       while (i < out_before && beg > bufbeg && beg != lastout)
         {
           ++i;
           do
             --beg;
           while (beg[-1] != eol);
         }
 
        
       if (beg != lastout)
         lastout = 0;
 
        
       save = residue + lim - beg;
       if (out_byte)
         totalcc = add_count (totalcc, buflim - bufbeg - save);
       if (out_line)
         nlscan (beg);
       if (! fillbuf (save, stats))
         {
           suppressible_error (filename, errno);
           goto finish_grep;
         }
     }
   if (residue)
     {
       *buflim++ = eol;
       if (outleft)
         nlines += grepbuf (bufbeg + save - residue, buflim);
       if (pending)
         prpending (buflim);
     }
 
  finish_grep:
   done_on_match -= not_text;
   out_quiet -= not_text;
   if ((not_text & ~out_quiet) && nlines != 0)
     printf (_("Binary file %s matches\n"), filename);
   return nlines;
 }
 
 static int
 grepfile (char const *file, struct stats *stats)
 {
   int desc;
   int count;
   int status;
  grepfile (char const *file, struct stats *stats)
  {
    int desc;
  int count;
//   intmax_t count;
    int status;
  
    filename = (file ? file : label ? label : _("(standard input)"));
        
       desc = -1;
     }
   else
     {
        
       desc = open (file, O_RDONLY);
       if (desc < 0 && directories != SKIP_DIRECTORIES)
         {
           suppressible_error (file, errno);
           return 1;
         }
     }
 
   if (desc < 0
       ? stat (file, &stats->stat) != 0
       : fstat (desc, &stats->stat) != 0)
     {
       suppressible_error (filename, errno);
       if (file)
         close (desc);
       return 1;
     }
 
   if ((directories == SKIP_DIRECTORIES && S_ISDIR (stats->stat.st_mode))
       || (devices == SKIP_DEVICES && (S_ISCHR (stats->stat.st_mode)
                                       || S_ISBLK (stats->stat.st_mode)
                                       || S_ISSOCK (stats->stat.st_mode)
                                       || S_ISFIFO (stats->stat.st_mode))))
     {
       if (file)
         close (desc);
       return 1;
     }
 
    
   if (!out_quiet && list_files == 0 && 1 < max_count
       && S_ISREG (out_stat.st_mode) && out_stat.st_ino
       && SAME_INODE (stats->stat, out_stat))
     {
       if (! suppress_errors)
         error (0, 0, _("input file %s is also the output"), quote (filename));
       errseen = 1;
       if (file)
         close (desc);
       return 1;
     }
 
   if (desc < 0)
     {
       desc = open (file, O_RDONLY);
       if (desc < 0)
         {
           suppressible_error (file, errno);
           return 1;
         }
     }
 
 #if defined SET_BINARY
    
   if (!isatty (desc))
     SET_BINARY (desc);
 #endif
 
   count = grep (desc, file, stats);
   if (count < 0)
     status = count + 2;
   else
     {
       if (count_matches)
         {
           if (out_file)
             {
               print_filename ();
               if (filename_mask)
                 print_sep (SEP_CHAR_SELECTED);
               else
                 fputc (0, stdout);
             }
           printf ("%d\n", count);
         }
                else
                  fputc (0, stdout);
              }
          printf ("%d\n", count);
//           printf ("%" PRIdMAX "\n", count);
          }
  
        status = !count;
 
       if (! file)
         {
           off_t required_offset = outleft ? bufoffset : after_last_match;
           if (required_offset != bufoffset
               && lseek (desc, required_offset, SEEK_SET) < 0
               && S_ISREG (stats->stat.st_mode))
             suppressible_error (filename, errno);
         }
       else
         while (close (desc) != 0)
           if (errno != EINTR)
             {
               suppressible_error (file, errno);
               break;
             }
     }