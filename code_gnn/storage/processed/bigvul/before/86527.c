static int test_string_formatting(void)
{
  int errors = 0;
  char buf[256];
  curl_msnprintf(buf, sizeof(buf), "%0*d%s", 2, 9, "foo");
  errors += string_check(buf, "09foo");

  curl_msnprintf(buf, sizeof(buf), "%*.*s", 5, 2, "foo");
  errors += string_check(buf, "   fo");

  curl_msnprintf(buf, sizeof(buf), "%*.*s", 2, 5, "foo");
  errors += string_check(buf, "foo");

  curl_msnprintf(buf, sizeof(buf), "%*.*s", 0, 10, "foo");
  errors += string_check(buf, "foo");

  curl_msnprintf(buf, sizeof(buf), "%-10s", "foo");
  errors += string_check(buf, "foo       ");

  curl_msnprintf(buf, sizeof(buf), "%10s", "foo");
  errors += string_check(buf, "       foo");

  curl_msnprintf(buf, sizeof(buf), "%*.*s", -10, -10, "foo");
  errors += string_check(buf, "foo       ");

  if(!errors)
    printf("All curl_mprintf() strings tests OK!\n");
  else
    printf("Some curl_mprintf() string tests Failed!\n");

  return errors;
}
