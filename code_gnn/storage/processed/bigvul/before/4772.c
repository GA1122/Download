g_utf8_to_ucs4_fast (const gchar * str, glong len, glong * items_written)
{
  gunichar *result;
  gsize n_chars, i;
  const gchar *p;

  g_return_val_if_fail (str != NULL, NULL);

  p = str;
  n_chars = 0;
  if (len < 0)
    {
      while (*p)
	{
	  p = g_utf8_next_char (p);
	  ++n_chars;
	}
    }
  else
    {
      while (p < str + len && *p)
	{
	  p = g_utf8_next_char (p);
	  ++n_chars;
	}
    }

  result = g_malloc (sizeof (gunichar) * (n_chars + 1));
  if (!result)
    return NULL;

  p = str;
  for (i = 0; i < n_chars; i++)
    {
      gunichar wc = (guchar) * p++;

      if (wc < 0x80)
	{
	  result[i] = wc;
	}
      else
	{
	  gunichar mask = 0x40;

	  if (G_UNLIKELY ((wc & mask) == 0))
	    {
	       
	      result[i] = 0xfffd;
	      continue;
	    }

	  do
	    {
	      wc <<= 6;
	      wc |= (guchar) (*p++) & 0x3f;
	      mask <<= 5;
	    }
	  while ((wc & mask) != 0);

	  wc &= mask - 1;

	  result[i] = wc;
	}
    }
  result[i] = 0;

  if (items_written)
    *items_written = i;

  return result;
}
