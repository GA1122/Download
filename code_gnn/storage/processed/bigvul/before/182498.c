  gplotMakeOutput(GPLOT  *gplot)
  {
char     buf[L_BUF_SIZE];
// char     buf[L_BUFSIZE];
  char    *cmdname;
  l_int32  ignore;
  
     PROCNAME("gplotMakeOutput");
 
     if (!gplot)
         return ERROR_INT("gplot not defined", procName, 1);
 
     gplotGenCommandFile(gplot);
     gplotGenDataFiles(gplot);
      cmdname = genPathname(gplot->cmdname, NULL);
  
  #ifndef _WIN32
    snprintf(buf, L_BUF_SIZE, "gnuplot %s", cmdname);
//     snprintf(buf, L_BUFSIZE, "gnuplot %s", cmdname);
  #else
    snprintf(buf, L_BUF_SIZE, "wgnuplot %s", cmdname);
//     snprintf(buf, L_BUFSIZE, "wgnuplot %s", cmdname);
  #endif   
  
  #ifndef OS_IOS  
     ignore = system(buf);   
 #endif  
 
     LEPT_FREE(cmdname);
     return 0;
 }