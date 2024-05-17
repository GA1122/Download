  int main(int argc, char **argv)
  {
   volatile int summary = 1;   
   volatile int memstats = 0;  
//    int summary = 1;   
//    int memstats = 0;  
  
      
   PNG_CONST char *volatile touch = NULL;
//    const char *touch = NULL;
  
      
  static double
       gammas[]={2.2, 1.0, 2.2/1.45, 1.8, 1.5, 2.4, 2.5, 2.62, 2.9};
 
   
  size_t cp = 0;
  char command[1024];
 
  
     anon_context(&pm.this);
  
//    gnu_volatile(summary)
//    gnu_volatile(memstats)
//    gnu_volatile(touch)
// 
      
     signal(SIGABRT, signal_handler);
     signal(SIGFPE, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGTERM, signal_handler);
 
 #ifdef HAVE_FEENABLEEXCEPT
   
    feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
 #endif
 
    modifier_init(&pm);
 
   
    store_ensure_image(&pm.this, NULL, 2, TRANSFORM_ROWMAX, TRANSFORM_HEIGHTMAX);
 
   
    cp = safecat(command, sizeof command, cp, "pngvalid");
 
   
    pm.this.treat_warnings_as_errors = 1;
 
   
    pm.assume_16_bit_calculations = PNG_LIBPNG_VER >= 10700;
 
   
    pm.calculations_use_input_precision = 1U;
 
  
      
     pm.gammas = gammas;
   pm.ngammas = (sizeof gammas) / (sizeof gammas[0]);
//    pm.ngammas = ARRAY_SIZE(gammas);
     pm.ngamma_tests = 0;  
  
//     
// #  ifdef PNG_WRITE_tRNS_SUPPORTED
//       pm.test_tRNS = 1;
// #  endif
//    pm.test_lbg = PNG_LIBPNG_VER >= 10600;
//    pm.test_lbg_gamma_threshold = 1;
//    pm.test_lbg_gamma_transform = PNG_LIBPNG_VER >= 10600;
//    pm.test_lbg_gamma_sbit = 1;
//    pm.test_lbg_gamma_composition = PNG_LIBPNG_VER >= 10700;
// 
      
     pm.encodings = test_encodings;
   pm.nencodings = (sizeof test_encodings) / (sizeof test_encodings[0]);
//    pm.nencodings = ARRAY_SIZE(test_encodings);
  
   pm.sbitlow = 8U;  
// #  if PNG_LIBPNG_VER < 10700
//       pm.sbitlow = 8U;  
// #  else
//       pm.sbitlow = 1U;
// #  endif
  
      
    pm.use_input_precision_16to8 = 1U;
    pm.use_input_precision_sbit = 1U;  
 
   
    pm.maxout8 = .1;  
    pm.maxabs8 = .00005;  
    pm.maxcalc8 = 1./255;  
    pm.maxpc8 = .499;  
 
     pm.maxout16 = .499;   
     pm.maxabs16 = .00005; 
     pm.maxcalc16 =1./65535; 
   pm.maxcalcG = 1./((1<<PNG_MAX_GAMMA_8)-1);
// #  if PNG_LIBPNG_VER < 10700
//       pm.maxcalcG = 1./((1<<PNG_MAX_GAMMA_8)-1);
// #  else
//       pm.maxcalcG = 1./((1<<16)-1);
// #  endif
  
      
    pm.maxpc16 = .005;  
 
   
  while (--argc >= 1)
  {
  int catmore = 0;  
 
   
       cp = safecat(command, sizeof command, cp, " ");
       cp = safecat(command, sizeof command, cp, *++argv);
 
  if (strcmp(*argv, "-v") == 0)
          pm.this.verbose = 1;
 
  else if (strcmp(*argv, "-l") == 0)
          pm.log = 1;
 
 
        else if (strcmp(*argv, "-q") == 0)
           summary = pm.this.verbose = pm.log = 0;
  
      else if (strcmp(*argv, "-w") == 0)
//       else if (strcmp(*argv, "-w") == 0 ||
//                strcmp(*argv, "--strict") == 0)
           pm.this.treat_warnings_as_errors = 0;
  
        else if (strcmp(*argv, "--speed") == 0)
          pm.this.speed = 1, pm.ngamma_tests = pm.ngammas, pm.test_standard = 0,
             summary = 0;
 
  else if (strcmp(*argv, "--memory") == 0)
          memstats = 1;
 
  else if (strcmp(*argv, "--size") == 0)
          pm.test_size = 1;
 
  else if (strcmp(*argv, "--nosize") == 0)
          pm.test_size = 0;
 
  else if (strcmp(*argv, "--standard") == 0)
          pm.test_standard = 1;
 
  else if (strcmp(*argv, "--nostandard") == 0)
          pm.test_standard = 0;
 
  else if (strcmp(*argv, "--transform") == 0)
          pm.test_transform = 1;
 
  else if (strcmp(*argv, "--notransform") == 0)
          pm.test_transform = 0;
 
 #ifdef PNG_READ_TRANSFORMS_SUPPORTED
  else if (strncmp(*argv, "--transform-disable=",
  sizeof "--transform-disable") == 0)
  {
          pm.test_transform = 1;
          transform_disable(*argv + sizeof "--transform-disable");
  }
 
  else if (strncmp(*argv, "--transform-enable=",
  sizeof "--transform-enable") == 0)
  {
          pm.test_transform = 1;
          transform_enable(*argv + sizeof "--transform-enable");
  }
 #endif  
 
  else if (strcmp(*argv, "--gamma") == 0)
  {
   
          pm.ngamma_tests = 2U;
          pm.test_gamma_threshold = 1;
 
           pm.test_gamma_transform = 1;
           pm.test_gamma_sbit = 1;
           pm.test_gamma_scale16 = 1;
         pm.test_gamma_background = 1;
//          pm.test_gamma_background = 1;  
           pm.test_gamma_alpha_mode = 1;
           }
  
  else if (strcmp(*argv, "--nogamma") == 0)
          pm.ngamma_tests = 0;
 
  else if (strcmp(*argv, "--gamma-threshold") == 0)
          pm.ngamma_tests = 2U, pm.test_gamma_threshold = 1;
 
  else if (strcmp(*argv, "--nogamma-threshold") == 0)
          pm.test_gamma_threshold = 0;
 
  else if (strcmp(*argv, "--gamma-transform") == 0)
          pm.ngamma_tests = 2U, pm.test_gamma_transform = 1;
 
  else if (strcmp(*argv, "--nogamma-transform") == 0)
          pm.test_gamma_transform = 0;
 
  else if (strcmp(*argv, "--gamma-sbit") == 0)
          pm.ngamma_tests = 2U, pm.test_gamma_sbit = 1;
 
  else if (strcmp(*argv, "--nogamma-sbit") == 0)
          pm.test_gamma_sbit = 0;
 
  else if (strcmp(*argv, "--gamma-16-to-8") == 0)
          pm.ngamma_tests = 2U, pm.test_gamma_scale16 = 1;
 
  else if (strcmp(*argv, "--nogamma-16-to-8") == 0)
          pm.test_gamma_scale16 = 0;
 
  else if (strcmp(*argv, "--gamma-background") == 0)
          pm.ngamma_tests = 2U, pm.test_gamma_background = 1;
 
  else if (strcmp(*argv, "--nogamma-background") == 0)
          pm.test_gamma_background = 0;
 
  else if (strcmp(*argv, "--gamma-alpha-mode") == 0)
          pm.ngamma_tests = 2U, pm.test_gamma_alpha_mode = 1;
 
  else if (strcmp(*argv, "--nogamma-alpha-mode") == 0)
          pm.test_gamma_alpha_mode = 0;
 
  else if (strcmp(*argv, "--expand16") == 0)
          pm.test_gamma_expand16 = 1;
 
 
        else if (strcmp(*argv, "--noexpand16") == 0)
           pm.test_gamma_expand16 = 0;
  
//       else if (strcmp(*argv, "--low-depth-gray") == 0)
//          pm.test_lbg = pm.test_lbg_gamma_threshold =
//             pm.test_lbg_gamma_transform = pm.test_lbg_gamma_sbit =
//             pm.test_lbg_gamma_composition = 1;
// 
//       else if (strcmp(*argv, "--nolow-depth-gray") == 0)
//          pm.test_lbg = pm.test_lbg_gamma_threshold =
//             pm.test_lbg_gamma_transform = pm.test_lbg_gamma_sbit =
//             pm.test_lbg_gamma_composition = 0;
// 
// #     ifdef PNG_WRITE_tRNS_SUPPORTED
//          else if (strcmp(*argv, "--tRNS") == 0)
//             pm.test_tRNS = 1;
// #     endif
// 
//       else if (strcmp(*argv, "--notRNS") == 0)
//          pm.test_tRNS = 0;
// 
        else if (strcmp(*argv, "--more-gammas") == 0)
           pm.ngamma_tests = 3U;
  
  else if (strcmp(*argv, "--all-gammas") == 0)
          pm.ngamma_tests = pm.ngammas;
 
  else if (strcmp(*argv, "--progressive-read") == 0)
          pm.this.progressive = 1;
 
  else if (strcmp(*argv, "--use-update-info") == 0)
  ++pm.use_update_info;  
 
  
        else if (strcmp(*argv, "--interlace") == 0)
        {
#        ifdef PNG_WRITE_INTERLACING_SUPPORTED
// #        if CAN_WRITE_INTERLACE
              pm.interlace_type = PNG_INTERLACE_ADAM7;
#        else
// #        else  
              fprintf(stderr, "pngvalid: no write interlace support\n");
              return SKIP;
#        endif
// #        endif  
        }
  
        else if (strcmp(*argv, "--use-input-precision") == 0)
          pm.use_input_precision = 1U;
 
  else if (strcmp(*argv, "--use-calculation-precision") == 0)
          pm.use_input_precision = 0;
 
  else if (strcmp(*argv, "--calculations-use-input-precision") == 0)
          pm.calculations_use_input_precision = 1U;
 
  else if (strcmp(*argv, "--assume-16-bit-calculations") == 0)
          pm.assume_16_bit_calculations = 1U;
 
  else if (strcmp(*argv, "--calculations-follow-bit-depth") == 0)
          pm.calculations_use_input_precision =
             pm.assume_16_bit_calculations = 0;
 
  else if (strcmp(*argv, "--exhaustive") == 0)
          pm.test_exhaustive = 1;
 
  else if (argc > 1 && strcmp(*argv, "--sbitlow") == 0)
  --argc, pm.sbitlow = (png_byte)atoi(*++argv), catmore = 1;
 
  else if (argc > 1 && strcmp(*argv, "--touch") == 0)
  --argc, touch = *++argv, catmore = 1;
 
  else if (argc > 1 && strncmp(*argv, "--max", 5) == 0)
  {
  --argc;
 
  if (strcmp(5+*argv, "abs8") == 0)
             pm.maxabs8 = atof(*++argv);
 
  else if (strcmp(5+*argv, "abs16") == 0)
             pm.maxabs16 = atof(*++argv);
 
  else if (strcmp(5+*argv, "calc8") == 0)
             pm.maxcalc8 = atof(*++argv);
 
  else if (strcmp(5+*argv, "calc16") == 0)
             pm.maxcalc16 = atof(*++argv);
 
  else if (strcmp(5+*argv, "out8") == 0)
             pm.maxout8 = atof(*++argv);
 
  else if (strcmp(5+*argv, "out16") == 0)
             pm.maxout16 = atof(*++argv);
 
  else if (strcmp(5+*argv, "pc8") == 0)
             pm.maxpc8 = atof(*++argv);
 
  else if (strcmp(5+*argv, "pc16") == 0)
             pm.maxpc16 = atof(*++argv);
 
  else
  {
             fprintf(stderr, "pngvalid: %s: unknown 'max' option\n", *argv);
             exit(99);
  }
 
          catmore = 1;
  }
 
  else if (strcmp(*argv, "--log8") == 0)
  --argc, pm.log8 = atof(*++argv), catmore = 1;
 
  else if (strcmp(*argv, "--log16") == 0)
  --argc, pm.log16 = atof(*++argv), catmore = 1;
 
 #ifdef PNG_SET_OPTION_SUPPORTED
  else if (strncmp(*argv, "--option=", 9) == 0)
  {
   
 
           const char *arg = 9+*argv;
           unsigned char option=0, setting=0;
  
#ifdef PNG_ARM_NEON_API_SUPPORTED
// #ifdef PNG_ARM_NEON
           if (strncmp(arg, "arm-neon:", 9) == 0)
              option = PNG_ARM_NEON, arg += 9;
  
           else
  #endif
// #ifdef PNG_EXTENSIONS
//          if (strncmp(arg, "extensions:", 11) == 0)
//             option = PNG_EXTENSIONS, arg += 11;
// 
//          else
// #endif
  #ifdef PNG_MAXIMUM_INFLATE_WINDOW
           if (strncmp(arg, "max-inflate-window:", 19) == 0)
              option = PNG_MAXIMUM_INFLATE_WINDOW, arg += 19;
 
  else
 #endif
  {
             fprintf(stderr, "pngvalid: %s: %s: unknown option\n", *argv, arg);
             exit(99);
  }
 
  if (strcmp(arg, "off") == 0)
             setting = PNG_OPTION_OFF;
 
  else if (strcmp(arg, "on") == 0)
             setting = PNG_OPTION_ON;
 
  else
  {
             fprintf(stderr,
  "pngvalid: %s: %s: unknown setting (use 'on' or 'off')\n",
  *argv, arg);
             exit(99);
  }
 
          pm.this.options[pm.this.noptions].option = option;
          pm.this.options[pm.this.noptions++].setting = setting;
  }
 #endif  
 
  else
  {
          fprintf(stderr, "pngvalid: %s: unknown argument\n", *argv);
          exit(99);
  }
 
  if (catmore)  
  {
          cp = safecat(command, sizeof command, cp, " ");
          cp = safecat(command, sizeof command, cp, *argv);
  }
  }
 
   
  if (pm.test_standard == 0 && pm.test_size == 0 && pm.test_transform == 0 &&
       pm.ngamma_tests == 0)
  {
   
       pm.test_standard = 1;
       pm.test_size = 1;
       pm.test_transform = 1;
       pm.ngamma_tests = 2U;
  }
 
  if (pm.ngamma_tests > 0 &&
       pm.test_gamma_threshold == 0 && pm.test_gamma_transform == 0 &&
       pm.test_gamma_sbit == 0 && pm.test_gamma_scale16 == 0 &&
       pm.test_gamma_background == 0 && pm.test_gamma_alpha_mode == 0)
  {
       pm.test_gamma_threshold = 1;
       pm.test_gamma_transform = 1;
       pm.test_gamma_sbit = 1;
       pm.test_gamma_scale16 = 1;
       pm.test_gamma_background = 1;
       pm.test_gamma_alpha_mode = 1;
  }
 
  else if (pm.ngamma_tests == 0)
  {
   
       pm.test_gamma_threshold = 0;
       pm.test_gamma_transform = 0;
       pm.test_gamma_sbit = 0;
       pm.test_gamma_scale16 = 0;
       pm.test_gamma_background = 0;
       pm.test_gamma_alpha_mode = 0;
  }
 
 
     Try
     {
         
      make_transform_images(&pm.this);
//       make_transform_images(&pm);
  
         
        if (pm.test_standard)
  {
          perform_interlace_macro_validation();
          perform_formatting_test(&pm.this);
 #        ifdef PNG_READ_SUPPORTED
             perform_standard_test(&pm);
 #        endif
          perform_error_test(&pm);
  }
 
   
  if (pm.test_size)
  {
          make_size_images(&pm.this);
 #        ifdef PNG_READ_SUPPORTED
             perform_size_test(&pm);
 #        endif
  }
 
 #ifdef PNG_READ_TRANSFORMS_SUPPORTED
   
  if (pm.test_transform)
          perform_transform_test(&pm);
 #endif  
 
 #ifdef PNG_READ_GAMMA_SUPPORTED
  if (pm.ngamma_tests > 0)
          perform_gamma_test(&pm, summary);
 #endif
  }
 
  Catch_anonymous
  {
       fprintf(stderr, "pngvalid: test aborted (probably failed in cleanup)\n");
  if (!pm.this.verbose)
  {
  if (pm.this.error[0] != 0)
             fprintf(stderr, "pngvalid: first error: %s\n", pm.this.error);
 
          fprintf(stderr, "pngvalid: run with -v to see what happened\n");
  }
       exit(1);
  }
 
  if (summary)
  {
       printf("%s: %s (%s point arithmetic)\n",
  (pm.this.nerrors || (pm.this.treat_warnings_as_errors &&
             pm.this.nwarnings)) ? "FAIL" : "PASS",
          command,
 #if defined(PNG_FLOATING_ARITHMETIC_SUPPORTED) || PNG_LIBPNG_VER < 10500
  "floating"
 #else
  "fixed"
 #endif
  );
  }
 
  if (memstats)
  {
       printf("Allocated memory statistics (in bytes):\n"
  "\tread  %lu maximum single, %lu peak, %lu total\n"
  "\twrite %lu maximum single, %lu peak, %lu total\n",
  (unsigned long)pm.this.read_memory_pool.max_max,
  (unsigned long)pm.this.read_memory_pool.max_limit,
  (unsigned long)pm.this.read_memory_pool.max_total,
  (unsigned long)pm.this.write_memory_pool.max_max,
  (unsigned long)pm.this.write_memory_pool.max_limit,
  (unsigned long)pm.this.write_memory_pool.max_total);
  }
 
   
    store_delete(&pm.this);
 
   
  if (pm.this.nerrors || (pm.this.treat_warnings_as_errors &&
        pm.this.nwarnings))
  {
  if (!pm.this.verbose)
          fprintf(stderr, "pngvalid: %s\n", pm.this.error);
 
       fprintf(stderr, "pngvalid: %d errors, %d warnings\n", pm.this.nerrors,
           pm.this.nwarnings);
 
       exit(1);
  }
 
   
  if (touch != NULL)
  {
  FILE *fsuccess = fopen(touch, "wt");
 
  if (fsuccess != NULL)
  {
  int error = 0;
          fprintf(fsuccess, "PNG validation succeeded\n");
          fflush(fsuccess);
          error = ferror(fsuccess);
 
  if (fclose(fsuccess) || error)
  {
             fprintf(stderr, "%s: write failed\n", touch);
             exit(1);
  }
  }
 
  else
  {
          fprintf(stderr, "%s: open failed\n", touch);
          exit(1);
  }
  }
 
   
    UNUSED(fail)
  return 0;
 }