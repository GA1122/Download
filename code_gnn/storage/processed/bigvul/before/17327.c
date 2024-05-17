set_default_locale ()
{
#if defined (HAVE_SETLOCALE)
  default_locale = setlocale (LC_ALL, "");
  if (default_locale)
    default_locale = savestring (default_locale);
#endif  
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  locale_mb_cur_max = MB_CUR_MAX;
}
