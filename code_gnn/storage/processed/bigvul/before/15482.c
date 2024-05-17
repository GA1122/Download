http_atotm (const char *time_string)
{
   

  static const char *time_formats[] = {
    "%a, %d %b %Y %T",           
    "%A, %d-%b-%y %T",           
    "%a %b %d %T %Y",            
    "%a, %d-%b-%Y %T"            
  };
  const char *oldlocale;
  char savedlocale[256];
  size_t i;
  time_t ret = (time_t) -1;

   
  oldlocale = setlocale (LC_TIME, NULL);
  if (oldlocale)
    {
      size_t l = strlen (oldlocale) + 1;
      if (l >= sizeof savedlocale)
        savedlocale[0] = '\0';
      else
        memcpy (savedlocale, oldlocale, l);
    }
  else savedlocale[0] = '\0';

  setlocale (LC_TIME, "C");

  for (i = 0; i < countof (time_formats); i++)
    {
      struct tm t;

       
      xzero (t);

      if (check_end (strptime (time_string, time_formats[i], &t)))
        {
          ret = timegm (&t);
          break;
        }
    }

   
  if (savedlocale[0])
    setlocale (LC_TIME, savedlocale);

  return ret;
}
