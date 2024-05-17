_g_utf8_normalize_wc (const gchar * str, gssize max_len, GNormalizeMode mode)
{
  gsize n_wc;
  gunichar *wc_buffer;
  const char *p;
  gsize last_start;
  gboolean do_compat = (mode == G_NORMALIZE_NFKC || mode == G_NORMALIZE_NFKD);
  gboolean do_compose = (mode == G_NORMALIZE_NFC || mode == G_NORMALIZE_NFKC);

  n_wc = 0;
  p = str;
  while ((max_len < 0 || p < str + max_len) && *p)
    {
      const gchar *decomp;
      gunichar wc = g_utf8_get_char (p);

      if (wc >= SBase && wc < SBase + SCount)
	{
	  gsize result_len;
	  decompose_hangul (wc, NULL, &result_len);
	  n_wc += result_len;
	}
      else
	{
	  decomp = find_decomposition (wc, do_compat);

	  if (decomp)
	    n_wc += g_utf8_strlen (decomp, -1);
	  else
	    n_wc++;
	}

      p = g_utf8_next_char (p);
    }

  wc_buffer = g_malloc (sizeof (gunichar) * (n_wc + 1));
  if (!wc_buffer)
    return NULL;

  last_start = 0;
  n_wc = 0;
  p = str;
  while ((max_len < 0 || p < str + max_len) && *p)
    {
      gunichar wc = g_utf8_get_char (p);
      const gchar *decomp;
      int cc;
      gsize old_n_wc = n_wc;

      if (wc >= SBase && wc < SBase + SCount)
	{
	  gsize result_len;
	  decompose_hangul (wc, wc_buffer + n_wc, &result_len);
	  n_wc += result_len;
	}
      else
	{
	  decomp = find_decomposition (wc, do_compat);

	  if (decomp)
	    {
	      const char *pd;
	      for (pd = decomp; *pd != '\0'; pd = g_utf8_next_char (pd))
		wc_buffer[n_wc++] = g_utf8_get_char (pd);
	    }
	  else
	    wc_buffer[n_wc++] = wc;
	}

      if (n_wc > 0)
	{
	  cc = COMBINING_CLASS (wc_buffer[old_n_wc]);

	  if (cc == 0)
	    {
	      g_unicode_canonical_ordering (wc_buffer + last_start,
					    n_wc - last_start);
	      last_start = old_n_wc;
	    }
	}

      p = g_utf8_next_char (p);
    }

  if (n_wc > 0)
    {
      g_unicode_canonical_ordering (wc_buffer + last_start,
				    n_wc - last_start);
    }

  wc_buffer[n_wc] = 0;

   

  if (do_compose && n_wc > 0)
    {
      gsize i, j;
      int last_cc = 0;
      last_start = 0;

      for (i = 0; i < n_wc; i++)
	{
	  int cc = COMBINING_CLASS (wc_buffer[i]);

	  if (i > 0 &&
	      (last_cc == 0 || last_cc != cc) &&
	      combine (wc_buffer[last_start], wc_buffer[i],
		       &wc_buffer[last_start]))
	    {
	      for (j = i + 1; j < n_wc; j++)
		wc_buffer[j - 1] = wc_buffer[j];
	      n_wc--;
	      i--;

	      if (i == last_start)
		last_cc = 0;
	      else
		last_cc = COMBINING_CLASS (wc_buffer[i - 1]);

	      continue;
	    }

	  if (cc == 0)
	    last_start = i;

	  last_cc = cc;
	}
    }

  wc_buffer[n_wc] = 0;

  return wc_buffer;
}
