load_cookies (void)
{
  if (!wget_cookie_jar)
    wget_cookie_jar = cookie_jar_new ();
  if (opt.cookies_input && !cookies_loaded_p)
    {
      cookie_jar_load (wget_cookie_jar, opt.cookies_input);
      cookies_loaded_p = true;
    }
}
