reset_locale_vars ()
{
  char *t;
#if defined (HAVE_SETLOCALE)
  if (lang == 0 || *lang == '\0')
    maybe_make_export_env ();		 
  if (setlocale (LC_ALL, lang ? lang : "") == 0)
    return 0;

#  if defined (LC_CTYPE)
  t = setlocale (LC_CTYPE, get_locale_var ("LC_CTYPE"));
#  endif
#  if defined (LC_COLLATE)
  t = setlocale (LC_COLLATE, get_locale_var ("LC_COLLATE"));
#  endif
#  if defined (LC_MESSAGES)
  t = setlocale (LC_MESSAGES, get_locale_var ("LC_MESSAGES"));
#  endif
#  if defined (LC_NUMERIC)
  t = setlocale (LC_NUMERIC, get_locale_var ("LC_NUMERIC"));
#  endif
#  if defined (LC_TIME)
  t = setlocale (LC_TIME, get_locale_var ("LC_TIME"));
#  endif

  locale_setblanks ();  
  locale_mb_cur_max = MB_CUR_MAX;
  u32reset ();

#endif
  return 1;
}
