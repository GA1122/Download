main(int argc, const char **argv)
{
 FILE *fp;
   png_uint_32 default_flags[4 ];
 int strict = 0, default_tests = 0;
 const char *count_argv = "default=save";
 const char *touch_file = NULL;
   display d;

   init_display(&d, argv[0]);

 while (++argv, --argc > 0)
 {
 if (strcmp(*argv, "--strict") == 0)
         strict = 1;

 else if (strcmp(*argv, "--default") == 0)
         default_tests = 1;

 else if (strcmp(*argv, "--touch") == 0)
 {
 if (argc > 1)
            touch_file = *++argv, --argc;

 else
            usage(d.program, "--touch: missing file name");
 }

 else
 break;
 }

  
 if (argc <= 0)
      usage(d.program, "missing test file");

  
 else if (default_tests) if (argc != 1)
      usage(d.program, "extra arguments");

  
   d.file = argv[--argc];

   fp = fopen(d.file, "rb");
 if (fp == NULL)
 {
      perror(d.file);
      exit(99);
 }

  
 if (check(fp, 1, &count_argv, default_flags, &d,
#     ifdef PNG_SAVE_UNKNOWN_CHUNKS_SUPPORTED
 0
#     else
 1
#     endif
 ) != PNG_HANDLE_CHUNK_ALWAYS)
 {
      fprintf(stderr, "%s: %s: internal error\n", d.program, d.file);
      exit(99);
 }

  
 if (!default_tests)
 {
      d.test = cmd;  
#     ifdef PNG_SAVE_UNKNOWN_CHUNKS_SUPPORTED
         perform_one_test(fp, argc, argv, default_flags, &d, 0);
#     endif
#     ifdef PNG_READ_USER_CHUNKS_SUPPORTED
         perform_one_test(fp, argc, argv, default_flags, &d, 1);
#     endif
      d.test = init;
 }

 else
 {
 const char **test = standard_tests;

  
 while (*test)
 {
 const char *this_test = *test++;
 const char **next = test;
 int count = display_rc(&d, strict), new_count;
 const char *result;
 int arg_count = 0;

 while (*next) ++next, ++arg_count;

         perform_one_test_safe(fp, arg_count, test, default_flags, &d,
            this_test);

         new_count = display_rc(&d, strict);

 if (new_count == count)
            result = "PASS";

 else
            result = "FAIL";

         printf("%s: %s %s\n", result, d.program, this_test);

         test = next+1;
 }
 }

   fclose(fp);

 if (display_rc(&d, strict) == 0)
 {
  
 if (touch_file != NULL)
 {
 FILE *fsuccess = fopen(touch_file, "wt");

 if (fsuccess != NULL)
 {
 int err = 0;
            fprintf(fsuccess, "PNG unknown tests succeeded\n");
            fflush(fsuccess);
            err = ferror(fsuccess);

 if (fclose(fsuccess) || err)
 {
               fprintf(stderr, "%s: write failed\n", touch_file);
               exit(99);
 }
 }

 else
 {
            fprintf(stderr, "%s: open failed\n", touch_file);
            exit(99);
 }
 }

 return 0;
 }

 return 1;
}
