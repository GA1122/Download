  bittok2str_internal(register const struct tok *lp, register const char *fmt,
  	   register u_int v, const char *sep)
  {
        static char buf[256];  
        int buflen=0;
//         static char buf[1024+1];  
//         char *bufp = buf;
//         size_t space_left = sizeof(buf), string_size;
          register u_int rotbit;  
          register u_int tokval;
          const char * sepstr = "";
 
 	while (lp != NULL && lp->s != NULL) {
             tokval=lp->v;    
             rotbit=1;
             while (rotbit != 0) {
                  
  		if (tokval == (v&rotbit)) {
                       
                    buflen+=snprintf(buf+buflen, sizeof(buf)-buflen, "%s%s",
                                     sepstr, lp->s);
//                     if (space_left <= 1)
//                         return (buf);  
//                     string_size = strlcpy(bufp, sepstr, space_left);
//                     if (string_size >= space_left)
//                         return (buf);     
//                     bufp += string_size;
//                     space_left -= string_size;
//                     if (space_left <= 1)
//                         return (buf);  
//                     string_size = strlcpy(bufp, lp->s, space_left);
//                     if (string_size >= space_left)
//                         return (buf);     
//                     bufp += string_size;
//                     space_left -= string_size;
                      sepstr = sep;
                      break;
                  }
                 rotbit=rotbit<<1;  
             }
              lp++;
  	}
  
        if (buflen == 0)
//         if (bufp == buf)
               
              (void)snprintf(buf, sizeof(buf), fmt == NULL ? "#%08x" : fmt, v);
          return (buf);
 }