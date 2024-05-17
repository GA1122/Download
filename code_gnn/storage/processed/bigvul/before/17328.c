set_default_locale_vars ()
{
  char *val;

#if defined (HAVE_SETLOCALE)

#  if defined (LC_CTYPE)
  val = get_string_value ("LC_CTYPE");
  if (val == 0 && lc_all && *lc_all)
    {
      setlocale (LC_CTYPE, lc_all);
      locale_setblanks ();
      locale_mb_cur_max = MB_CUR_MAX;
      u32reset ();
    }
#  endif

#  if defined (LC_COLLATE)
  val = get_string_value ("LC_COLLATE");
  if (val == 0 && lc_all && *lc_all)
    setlocale (LC_COLLATE, lc_all);
#  endif  

#  if defined (LC_MESSAGES)
  val = get_string_value ("LC_MESSAGES");
  if (val == 0 && lc_all && *lc_all)
    setlocale (LC_MESSAGES, lc_all);
#  endif  

#  if defined (LC_NUMERIC)
  val = get_string_value ("LC_NUMERIC");
  if (val == 0 && lc_all && *lc_all)
    setlocale (LC_NUMERIC, lc_all);
#  endif  

#  if defined (LC_TIME)
  val = get_string_value ("LC_TIME");
  if (val == 0 && lc_all && *lc_all)
    setlocale (LC_TIME, lc_all);
#  endif  

#endif  

  val = get_string_value ("TEXTDOMAIN");
  if (val && *val)
    {
      FREE (default_domain);
      default_domain = savestring (val);
#if 0
       
      textdomain (default_domain);
#endif
    }

  val = get_string_value ("TEXTDOMAINDIR");
  if (val && *val)
    {
      FREE (default_dir);
      default_dir = savestring (val);
      if (default_domain && *default_domain)
	bindtextdomain (default_domain, default_dir);
    }
}
