do_test(struct display *dp, const char *file)
  
{
 int ret = setjmp(dp->error_return);

 if (ret == 0)
 {
      test_one_file(dp, file);
 return 0;
 }

 else if (ret < ERRORS)  
      display_log(dp, INTERNAL_ERROR, "unexpected return code %d", ret);

 return ret;
}
