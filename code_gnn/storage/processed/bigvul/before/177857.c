 plan_a (char const *filename)
 {
   char const *s;
   char const *lim;
   char const **ptr;
   char *buffer;
   lin iline;
   size_t size = instat.st_size;
 
    
   if (! (size == instat.st_size
 	 && (buffer = malloc (size ? size : (size_t) 1))))
     return false;
 
    
   if (size)
      {
        if (S_ISREG (instat.st_mode))
          {
	  int ifd = safe_open (filename, O_RDONLY|binary_transput, 0);
// 	  int flags = O_RDONLY | binary_transput;
  	  size_t buffered = 0, n;
// 	  int ifd;
// 
// 	  if (! follow_symlinks)
// 	    flags |= O_NOFOLLOW;
// 	  ifd = safe_open (filename, flags, 0);
  	  if (ifd < 0)
  	    pfatal ("can't open file %s", quotearg (filename));
  
 		   
 		  size = buffered;
 		  break;
 		}
 	      if (n == (size_t) -1)
 		{
 		   
 		  close (ifd);
 		  free (buffer);
 		  return false;
 		}
 	      buffered += n;
 	    }
 
 	  if (close (ifd) != 0)
 	    read_fatal ();
 	}