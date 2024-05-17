g_utf8_strlen (const gchar * p, gssize max)
{
  glong len = 0;
  const gchar *start = p;
  g_return_val_if_fail (p != NULL || max == 0, 0);

  if (max < 0)
    {
      while (*p)
	{
	  p = g_utf8_next_char (p);
	  ++len;
	}
    }
  else
    {
      if (max == 0 || !*p)
	return 0;

      p = g_utf8_next_char (p);

      while (p - start < max && *p)
	{
	  ++len;
	  p = g_utf8_next_char (p);
	}

       
      if (p - start <= max)
	++len;
    }

  return len;
}
