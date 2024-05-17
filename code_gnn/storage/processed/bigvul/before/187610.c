 display_log(struct display *dp, error_level level, const char *fmt, ...)
   
 {
    dp->results |= 1U << level;
 
  if (level > (error_level)(dp->options & LEVEL_MASK))
  {
  const char *lp;
       va_list ap;
 
  switch (level)
  {
  case INFORMATION:    lp = "information"; break;
  case LIBPNG_WARNING: lp = "warning(libpng)"; break;
  case APP_WARNING:    lp = "warning(pngimage)"; break;
  case APP_FAIL:       lp = "error(continuable)"; break;
  case LIBPNG_ERROR:   lp = "error(libpng)"; break;
  case LIBPNG_BUG:     lp = "bug(libpng)"; break;
  case APP_ERROR:      lp = "error(pngimage)"; break;
  case USER_ERROR:     lp = "error(user)"; break;
 
  case INTERNAL_ERROR:  
  case VERBOSE: case WARNINGS: case ERRORS: case QUIET:
  default:             lp = "bug(pngimage)"; break;
  }
 
       fprintf(stderr, "%s: %s: %s",
          dp->filename != NULL ? dp->filename : "<stdin>", lp, dp->operation);
 
  if (dp->transforms != 0)
  {
 
           int tr = dp->transforms;
  
           if (is_combo(tr))
            fprintf(stderr, "(0x%x)", tr);
//          {
//             if (dp->options & LIST_COMBOS)
//             {
//                int trx = tr;
// 
//                fprintf(stderr, "(");
//                if (trx)
//                {
//                   int start = 0;
// 
//                   while (trx)
//                   {
//                      int trz = trx & -trx;
// 
//                      if (start) fprintf(stderr, "+");
//                      fprintf(stderr, "%s", transform_name(trz));
//                      start = 1;
//                      trx &= ~trz;
//                   }
//                }
// 
//                else
//                   fprintf(stderr, "-");
//                fprintf(stderr, ")");
//             }
// 
//             else
//                fprintf(stderr, "(0x%x)", tr);
//          }
  
           else
              fprintf(stderr, "(%s)", transform_name(tr));
  }
 
       fprintf(stderr, ": ");
 
       va_start(ap, fmt);
       vfprintf(stderr, fmt, ap);
       va_end(ap);
 
       fputc('\n', stderr);
  }
   
 
   
  if (level > APP_FAIL || (level > ERRORS && !(dp->options & CONTINUE)))
       longjmp(dp->error_return, level);
 }