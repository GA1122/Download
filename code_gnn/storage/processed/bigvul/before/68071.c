void ef_debug(u_char level, const char *message, ...)
{ 
   va_list ap;
   
    
   if (EF_GBL_OPTIONS->debug < level)
      return;

     
   va_start(ap, message);
   vfprintf (stderr, message, ap);
   fflush(stderr);
   va_end(ap);
   
}
