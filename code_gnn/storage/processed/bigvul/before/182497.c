  gplotGenCommandFile(GPLOT  *gplot)
  {
char     buf[L_BUF_SIZE];
// char     buf[L_BUFSIZE];
  char    *cmdstr, *plottitle, *dataname;
  l_int32  i, plotstyle, nplots;
  FILE    *fp;
 
     PROCNAME("gplotGenCommandFile");
 
     if (!gplot)
         return ERROR_INT("gplot not defined", procName, 1);
 
          
     sarrayClear(gplot->cmddata);
  
           
      if (gplot->title) {    
        snprintf(buf, L_BUF_SIZE, "set title '%s'", gplot->title);
//         snprintf(buf, L_BUFSIZE, "set title '%s'", gplot->title);
          sarrayAddString(gplot->cmddata, buf, L_COPY);
      }
      if (gplot->xlabel) {    
        snprintf(buf, L_BUF_SIZE, "set xlabel '%s'", gplot->xlabel);
//         snprintf(buf, L_BUFSIZE, "set xlabel '%s'", gplot->xlabel);
          sarrayAddString(gplot->cmddata, buf, L_COPY);
      }
      if (gplot->ylabel) {    
        snprintf(buf, L_BUF_SIZE, "set ylabel '%s'", gplot->ylabel);
//         snprintf(buf, L_BUFSIZE, "set ylabel '%s'", gplot->ylabel);
          sarrayAddString(gplot->cmddata, buf, L_COPY);
      }
  
           
      if (gplot->outformat == GPLOT_PNG) {
        snprintf(buf, L_BUF_SIZE, "set terminal png; set output '%s'",
//         snprintf(buf, L_BUFSIZE, "set terminal png; set output '%s'",
                   gplot->outname);
      } else if (gplot->outformat == GPLOT_PS) {
        snprintf(buf, L_BUF_SIZE, "set terminal postscript; set output '%s'",
//         snprintf(buf, L_BUFSIZE, "set terminal postscript; set output '%s'",
                   gplot->outname);
      } else if (gplot->outformat == GPLOT_EPS) {
        snprintf(buf, L_BUF_SIZE,
//         snprintf(buf, L_BUFSIZE,
                   "set terminal postscript eps; set output '%s'",
                   gplot->outname);
      } else if (gplot->outformat == GPLOT_LATEX) {
        snprintf(buf, L_BUF_SIZE, "set terminal latex; set output '%s'",
//         snprintf(buf, L_BUFSIZE, "set terminal latex; set output '%s'",
                   gplot->outname);
      }
      sarrayAddString(gplot->cmddata, buf, L_COPY);
  
      if (gplot->scaling == GPLOT_LOG_SCALE_X ||
          gplot->scaling == GPLOT_LOG_SCALE_X_Y) {
        snprintf(buf, L_BUF_SIZE, "set logscale x");
//         snprintf(buf, L_BUFSIZE, "set logscale x");
          sarrayAddString(gplot->cmddata, buf, L_COPY);
      }
      if (gplot->scaling == GPLOT_LOG_SCALE_Y ||
          gplot->scaling == GPLOT_LOG_SCALE_X_Y) {
        snprintf(buf, L_BUF_SIZE, "set logscale y");
//         snprintf(buf, L_BUFSIZE, "set logscale y");
          sarrayAddString(gplot->cmddata, buf, L_COPY);
      }
  
     nplots = sarrayGetCount(gplot->datanames);
     for (i = 0; i < nplots; i++) {
         plottitle = sarrayGetString(gplot->plottitles, i, L_NOCOPY);
          dataname = sarrayGetString(gplot->datanames, i, L_NOCOPY);
          numaGetIValue(gplot->plotstyles, i, &plotstyle);
          if (nplots == 1) {
            snprintf(buf, L_BUF_SIZE, "plot '%s' title '%s' %s",
//             snprintf(buf, L_BUFSIZE, "plot '%s' title '%s' %s",
                       dataname, plottitle, gplotstylenames[plotstyle]);
          } else {
              if (i == 0)
                snprintf(buf, L_BUF_SIZE, "plot '%s' title '%s' %s, \\",
//                 snprintf(buf, L_BUFSIZE, "plot '%s' title '%s' %s, \\",
                       dataname, plottitle, gplotstylenames[plotstyle]);
              else if (i < nplots - 1)
                snprintf(buf, L_BUF_SIZE, " '%s' title '%s' %s, \\",
//                 snprintf(buf, L_BUFSIZE, " '%s' title '%s' %s, \\",
                       dataname, plottitle, gplotstylenames[plotstyle]);
              else
                snprintf(buf, L_BUF_SIZE, " '%s' title '%s' %s",
//                 snprintf(buf, L_BUFSIZE, " '%s' title '%s' %s",
                       dataname, plottitle, gplotstylenames[plotstyle]);
          }
          sarrayAddString(gplot->cmddata, buf, L_COPY);
     }
 
          
     cmdstr = sarrayToString(gplot->cmddata, 1);
     if ((fp = fopenWriteStream(gplot->cmdname, "w")) == NULL) {
         LEPT_FREE(cmdstr);
         return ERROR_INT("cmd stream not opened", procName, 1);
     }
     fwrite(cmdstr, 1, strlen(cmdstr), fp);
     fclose(fp);
     LEPT_FREE(cmdstr);
     return 0;
 }