 int main(int argc, char **argv)
 {
 #ifdef sgi
  char tmpline[80];
 #endif
  char *p, *bgstr = NULL;
  int rc, alen, flen;
  int error = 0;
  int timing = FALSE;
  int have_bg = FALSE;
 #ifdef FEATURE_LOOP
  int loop = FALSE;
  long loop_interval = -1;  
 #endif
  double LUT_exponent;  
  double CRT_exponent = 2.2;  
  double default_display_exponent;  
  XEvent e;
  KeySym k;
 
 
   
 
     displayname = (char *)NULL;
     filename = (char *)NULL;
     memset(&rpng2_info, 0, sizeof(mainprog_info));
 
 
   
 
 #if defined(NeXT)
   
     LUT_exponent = 1.0 / 2.2;
   
 #elif defined(sgi)
     LUT_exponent = 1.0 / 1.7;
   
     infile = fopen("/etc/config/system.glGammaVal", "r");
  if (infile) {
  double sgi_gamma;
 
         fgets(tmpline, 80, infile);
         fclose(infile);
         sgi_gamma = atof(tmpline);
  if (sgi_gamma > 0.0)
             LUT_exponent = 1.0 / sgi_gamma;
  }
 #elif defined(Macintosh)
     LUT_exponent = 1.8 / 2.61;
   
 #else
     LUT_exponent = 1.0;  
 #endif
 
   
     default_display_exponent = LUT_exponent * CRT_exponent;
 
 
   
 
  if ((p = getenv("SCREEN_GAMMA")) != NULL)
         rpng2_info.display_exponent = atof(p);
  else
         rpng2_info.display_exponent = default_display_exponent;
 
 
   
 
  while (*++argv && !error) {
  if (!strncmp(*argv, "-display", 2)) {
  if (!*++argv)
  ++error;
  else
                 displayname = *argv;
  } else if (!strncmp(*argv, "-gamma", 2)) {
  if (!*++argv)
  ++error;
  else {
                 rpng2_info.display_exponent = atof(*argv);
  if (rpng2_info.display_exponent <= 0.0)
  ++error;
  }
  } else if (!strncmp(*argv, "-bgcolor", 4)) {
  if (!*++argv)
  ++error;
  else {
                 bgstr = *argv;
  if (strlen(bgstr) != 7 || bgstr[0] != '#')
  ++error;
  else {
                     have_bg = TRUE;
                     bg_image = FALSE;
  }
  }
  } else if (!strncmp(*argv, "-bgpat", 4)) {
  if (!*++argv)
  ++error;
  else {
                 pat = atoi(*argv);
  if (pat >= 0 && pat < num_bgpat) {
                     bg_image = TRUE;
                     have_bg = FALSE;
  } else
  ++error;
  }
  } else if (!strncmp(*argv, "-usleep", 2)) {
  if (!*++argv)
  ++error;
  else {
                 usleep_duration = (ulg)atol(*argv);
                 demo_timing = TRUE;
  }
  } else if (!strncmp(*argv, "-pause", 2)) {
             pause_after_pass = TRUE;
  } else if (!strncmp(*argv, "-timing", 2)) {
             timing = TRUE;
 #ifdef FEATURE_LOOP
  } else if (!strncmp(*argv, "-loop", 2)) {
             loop = TRUE;
  if (!argv[1] || !is_number(argv[1]))
                 loop_interval = 2;
  else {
  ++argv;
                 loop_interval = atol(*argv);
  if (loop_interval < 0)
                     loop_interval = 2;
  else if (loop_interval > 100000)  
                     loop_interval = 100000;
  }
 #endif
  } else {
  if (**argv != '-') {
                 filename = *argv;
  if (argv[1])  
  ++error;
  } else
  ++error;  
  }
  }
 
  if (!filename)
  ++error;
 
 
   
 
  if (error) {
 
          fprintf(stderr, "\n%s %s:  %s\n\n", PROGNAME, VERSION, appname);
          readpng2_version_info();
          fprintf(stderr, "\n"
          "Usage:  %s [-display xdpy] [-gamma exp] [-bgcolor bg | -bgpat pat]\n"
//           "Usage:   ");
//         fprintf(stderr,
//           "%s [-display xdpy] [-gamma exp] [-bgcolor bg | -bgpat pat]\n"
//           "        %*s [-usleep dur | -timing] [-pause]\n",
//           PROGNAME, (int)strlen(PROGNAME), " ");
//         fprintf(stderr,
  #ifdef FEATURE_LOOP
          "        %*s [-usleep dur | -timing] [-pause] [-loop [sec]] file.png\n\n"
#else
          "        %*s [-usleep dur | -timing] [-pause] file.png\n\n"
//           "        [-loop [sec]]"
  #endif
//           " file.png\n\n");
//         fprintf(stderr,
            "    xdpy\tname of the target X display (e.g., ``hostname:0'')\n"
            "    exp \ttransfer-function exponent (``gamma'') of the display\n"
            "\t\t  system in floating-point format (e.g., ``%.1f''); equal\n"
          "\t\t  to the product of the lookup-table exponent (varies)\n"
//           "\t\t  to the product of the lookup-table exponent (varies)\n",
//           default_display_exponent);
//         fprintf(stderr,
            "\t\t  and the CRT exponent (usually 2.2); must be positive\n"
            "    bg  \tdesired background color in 7-character hex RGB format\n"
            "\t\t  (e.g., ``#ff7700'' for orange:  same as HTML colors);\n"
            "\t\t  used with transparent images; overrides -bgpat\n"
            "    pat \tdesired background pattern number (0-%d); used with\n"
          "\t\t  transparent images; overrides -bgcolor\n"
//           "\t\t  transparent images; overrides -bgcolor\n",
//           num_bgpat-1);
  #ifdef FEATURE_LOOP
//         fprintf(stderr,
            "    -loop\tloops through background images after initial display\n"
            "\t\t  is complete (depends on -bgpat)\n"
          "    sec \tseconds to display each background image (default = 2)\n"
//           "    sec \tseconds to display each background image (default = 2)\n");
  #endif
//         fprintf(stderr,
            "    dur \tduration in microseconds to wait after displaying each\n"
            "\t\t  row (for demo purposes)\n"
            "    -timing\tenables delay for every block read, to simulate modem\n"
            "\t\t  download of image (~36 Kbps)\n"
            "    -pause\tpauses after displaying each pass until mouse clicked\n"
            "\nPress Q, Esc or mouse button 1 (within image window, after image\n"
          "is displayed) to quit.\n"
          "\n", PROGNAME,
          (int)strlen(PROGNAME), " ", default_display_exponent, num_bgpat-1);
//           "is displayed) to quit.\n");
          exit(1);
      }
  
      if (!(infile = fopen(filename, "rb"))) {
          fprintf(stderr, PROGNAME ":  can't open PNG file [%s]\n", filename);
          ++error;
  } else {
         incount = fread(inbuf, 1, INBUFSIZE, infile);
  if (incount < 8 || !readpng2_check_sig(inbuf, 8)) {
             fprintf(stderr, PROGNAME
  ":  [%s] is not a PNG file: incorrect signature\n",
               filename);
  ++error;
  } else if ((rc = readpng2_init(&rpng2_info)) != 0) {
  switch (rc) {
  case 2:
                     fprintf(stderr, PROGNAME
  ":  [%s] has bad IHDR (libpng longjmp)\n", filename);
  break;
  case 4:
                     fprintf(stderr, PROGNAME ":  insufficient memory\n");
  break;
  default:
                     fprintf(stderr, PROGNAME
  ":  unknown readpng2_init() error\n");
  break;
  }
  ++error;
  } else {
  Trace((stderr, "about to call XOpenDisplay()\n"))
             display = XOpenDisplay(displayname);
  if (!display) {
                 readpng2_cleanup(&rpng2_info);
                 fprintf(stderr, PROGNAME ":  can't open X display [%s]\n",
                   displayname? displayname : "default");
  ++error;
  }
  }
  if (error)
             fclose(infile);
  }
 
 
  if (error) {
         fprintf(stderr, PROGNAME ":  aborting.\n");
         exit(2);
  }
 
 
   
 
     alen = strlen(appname);
     flen = strlen(filename);
  if (alen + flen + 3 > 1023)
         sprintf(titlebar, "%s:  ...%s", appname, filename+(alen+flen+6-1023));
  else
         sprintf(titlebar, "%s:  %s", appname, filename);
 
 
   
 
  if (have_bg) {
  unsigned r, g, b;  
 
         sscanf(bgstr+1, "%2x%2x%2x", &r, &g, &b);
         rpng2_info.bg_red   = (uch)r;
         rpng2_info.bg_green = (uch)g;
         rpng2_info.bg_blue  = (uch)b;
  } else
         rpng2_info.need_bgcolor = TRUE;
 
     rpng2_info.state = kPreInit;
     rpng2_info.mainprog_init = rpng2_x_init;
     rpng2_info.mainprog_display_row = rpng2_x_display_row;
     rpng2_info.mainprog_finish_display = rpng2_x_finish_display;
 
 
   
 
  for (;;) {
  Trace((stderr, "about to call readpng2_decode_data()\n"))
  if (readpng2_decode_data(&rpng2_info, inbuf, incount))
  ++error;
  Trace((stderr, "done with readpng2_decode_data()\n"))
 
  if (error || incount != INBUFSIZE || rpng2_info.state == kDone) {
  if (rpng2_info.state == kDone) {
  Trace((stderr, "done decoding PNG image\n"))
  } else if (ferror(infile)) {
                 fprintf(stderr, PROGNAME
  ":  error while reading PNG image file\n");
                 exit(3);
  } else if (feof(infile)) {
                 fprintf(stderr, PROGNAME ":  end of file reached "
  "(unexpectedly) while reading PNG image file\n");
                 exit(3);
  } else   {
   
  }
  break;
  }
 
  if (timing)
             sleep(1);
 
         incount = fread(inbuf, 1, INBUFSIZE, infile);
  }
 
 
   
 
     fclose(infile);
  Trace((stderr, "about to call readpng2_cleanup()\n"))
     readpng2_cleanup(&rpng2_info);
 
  if (error) {
         fprintf(stderr, PROGNAME ":  libpng error while decoding PNG image\n");
         exit(3);
  }
 
 
 #ifdef FEATURE_LOOP
 
  if (loop && bg_image) {
  Trace((stderr, "entering -loop loop (FEATURE_LOOP)\n"))
  for (;;) {
  int i, use_sleep;
  struct timeval now, then;
 
   
  if (gettimeofday(&then, NULL) == 0) {
                 then.tv_sec += loop_interval;
                 use_sleep = FALSE;
  } else
                 use_sleep = TRUE;
 
   
   
  if (XCheckMaskEvent(display, KeyPressMask | ButtonPressMask, &e))
  if (QUIT(e,k))
  break;
 
   
  if (++pat >= num_bgpat)
                 pat = 0;
             rpng2_x_reload_bg_image();
 
   
  if (use_sleep || gettimeofday(&now, NULL) != 0) {
  for (i = loop_interval;  i > 0; --i) {
                     sleep(1);
   
  if (XCheckMaskEvent(display, KeyPressMask | ButtonPressMask,
  &e) && QUIT(e,k))
  break;
  }
  } else {
   
  if (now.tv_sec < then.tv_sec ||
  (now.tv_sec == then.tv_sec && now.tv_usec < then.tv_usec))
  {
  int quit = FALSE;
  long seconds_to_go = then.tv_sec - now.tv_sec;
  long usleep_usec;
 
   
  while (seconds_to_go-- > 1) {
  int seconds_done = 0;
 
  for (i = seconds_to_go;  i > 0 && !quit; --i) {
                             sleep(1);
   
  if (XCheckMaskEvent(display, KeyPressMask |
  ButtonPressMask, &e) && QUIT(e,k))
                                 quit = TRUE;
  if (++seconds_done > 1000)
  break;  
  }
  if (quit)
  break;
 
   
  if (gettimeofday(&now, NULL) == 0) {
  if (now.tv_sec >= then.tv_sec)
  break;
                             seconds_to_go = then.tv_sec - now.tv_sec;
  } else
  ++seconds_to_go;  
  }
  if (quit)
  break;  
 
   
  if (gettimeofday(&now, NULL) == 0) {
                         usleep_usec = 1000000L*(then.tv_sec - now.tv_sec) +
                           then.tv_usec - now.tv_usec;
  if (usleep_usec > 0)
                             usleep((ulg)usleep_usec);
  }
  }
  }
 
   
             rpng2_x_redisplay_image (0, 0, rpng2_info.width, rpng2_info.height);
  }
 
  } else  
 
 #endif  
 
   
 
  if (rpng2_info.state >= kWindowInit) {
  Trace((stderr, "entering final wait-for-quit-event loop\n"))
  do {
  XNextEvent(display, &e);
  if (e.type == Expose) {
  XExposeEvent *ex = (XExposeEvent *)&e;
                 rpng2_x_redisplay_image (ex->x, ex->y, ex->width, ex->height);
  }
  } while (!QUIT(e,k));
  } else {
         fprintf(stderr, PROGNAME ":  init callback never called:  probable "
  "libpng error while decoding PNG metadata\n");
         exit(4);
  }
 
 
   
 
 
      Trace((stderr, "about to call rpng2_x_cleanup()\n"))
      rpng2_x_cleanup();
  
//     (void)argc;  
// 
      return 0;
  }