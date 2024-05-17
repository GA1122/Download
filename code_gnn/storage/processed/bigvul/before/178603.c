  stub_charset ()
  {
   locale = get_locale_var ("LC_CTYPE");
  
    locale = get_locale_var ("LC_CTYPE");
    if (locale == 0 || *locale == 0)
    return "ASCII";
//     {
//       strcpy (charsetbuf, "ASCII");
//       return charsetbuf;
//     }
    s = strrchr (locale, '.');
    if (s)
      {
      t = strchr (s, '@');
//       strcpy (charsetbuf, s+1);
//       t = strchr (charsetbuf, '@');
        if (t)
  	*t = 0;
      return ++s;
//       return charsetbuf;
      }
  else if (STREQ (locale, "UTF-8"))
    return "UTF-8";
  else
    return "ASCII";
//   strcpy (charsetbuf, locale);
//   return charsetbuf;
  }