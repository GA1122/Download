 int main(int argc, char **argv)
 {
 #ifdef sgi
  char tmpline[80];
 #endif
  char *p;
  int rc, alen, flen;
  int error = 0;
  int have_bg = FALSE;
  double LUT_exponent;  
  double CRT_exponent = 2.2;  
  double default_display_exponent;  
  XEvent e;
  KeySym k;
 
 
     displayname = (char *)NULL;
     filename = (char *)NULL;
 
 
   
 
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
         display_exponent = atof(p);
  else
         display_exponent = default_display_exponent;
 
 
   
 
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
                 display_exponent = atof(*argv);
  if (display_exponent <= 0.0)
  ++error;
  }
  } else if (!strncmp(*argv, "-bgcolor", 2)) {
  if (!*++argv)
  ++error;
  else {
                 bgstr = *argv;
  if (strlen(bgstr) != 7 || bgstr[0] != '#')
  ++error;
  else
                     have_bg = TRUE;
  }
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
         fprintf(stderr, "\n%s %s:  %s\n", PROGNAME, VERSION, appname);
         readpng_version_info();
         fprintf(stderr, "\n"
 
            "Usage:  %s [-display xdpy] [-gamma exp] [-bgcolor bg] file.png\n"
            "    xdpy\tname of the target X display (e.g., ``hostname:0'')\n"
            "    exp \ttransfer-function exponent (``gamma'') of the display\n"
          "\t\t  system in floating-point format (e.g., ``%.1f''); equal\n"
//           "\t\t  system in floating-point format (e.g., ``%.1f''); equal\n",
//           PROGNAME, default_display_exponent);
// 
//         fprintf(stderr, "\n"
            "\t\t  to the product of the lookup-table exponent (varies)\n"
            "\t\t  and the CRT exponent (usually 2.2); must be positive\n"
            "    bg  \tdesired background color in 7-character hex RGB format\n"
            "\t\t  (e.g., ``#ff7700'' for orange:  same as HTML colors);\n"
            "\t\t  used with transparent images\n"
            "\nPress Q, Esc or mouse button 1 (within image window, after image\n"
          "is displayed) to quit.\n"
          "\n", PROGNAME, default_display_exponent);
//           "is displayed) to quit.\n");
          exit(1);
      }
  
 
  if (!(infile = fopen(filename, "rb"))) {
         fprintf(stderr, PROGNAME ":  can't open PNG file [%s]\n", filename);
  ++error;
  } else {
  if ((rc = readpng_init(infile, &image_width, &image_height)) != 0) {
  switch (rc) {
  case 1:
                     fprintf(stderr, PROGNAME
  ":  [%s] is not a PNG file: incorrect signature\n",
                       filename);
  break;
  case 2:
                     fprintf(stderr, PROGNAME
  ":  [%s] has bad IHDR (libpng longjmp)\n", filename);
  break;
  case 4:
                     fprintf(stderr, PROGNAME ":  insufficient memory\n");
  break;
  default:
                     fprintf(stderr, PROGNAME
  ":  unknown readpng_init() error\n");
  break;
  }
  ++error;
  } else {
             display = XOpenDisplay(displayname);
  if (!display) {
                 readpng_cleanup(TRUE);
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
         bg_red   = (uch)r;
         bg_green = (uch)g;
         bg_blue  = (uch)b;
  } else if (readpng_get_bgcolor(&bg_red, &bg_green, &bg_blue) > 1) {
         readpng_cleanup(TRUE);
         fprintf(stderr, PROGNAME
  ":  libpng error while checking for background color\n");
         exit(2);
  }
 
 
   
 
  if (rpng_x_create_window())
         exit(2);
 
 
   
 
  Trace((stderr, "calling readpng_get_image()\n"))
     image_data = readpng_get_image(display_exponent, &image_channels,
  &image_rowbytes);
  Trace((stderr, "done with readpng_get_image()\n"))
 
 
   
 
     readpng_cleanup(FALSE);
     fclose(infile);
 
  if (!image_data) {
         fprintf(stderr, PROGNAME ":  unable to decode PNG image\n");
         exit(3);
  }
 
 
   
 
  Trace((stderr, "calling rpng_x_display_image()\n"))
  if (rpng_x_display_image()) {
         free(image_data);
         exit(4);
  }
  Trace((stderr, "done with rpng_x_display_image()\n"))
 
 
   
 
     printf(
  "Done.  Press Q, Esc or mouse button 1 (within image window) to quit.\n");
     fflush(stdout);
 
  do
  XNextEvent(display, &e);
  while (!(e.type == ButtonPress && e.xbutton.button == Button1) &&
  !(e.type == KeyPress &&  
  ((k = XLookupKeysym(&e.xkey, 0)) == XK_q || k == XK_Escape) ));
 
 
   
 
  
      rpng_x_cleanup();
  
//     (void)argc;  
// 
      return 0;
  }