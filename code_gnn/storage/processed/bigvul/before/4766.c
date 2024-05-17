g_ucs4_to_utf8 (const gunichar * str,
		glong len,
		glong * items_read, glong * items_written)
{
  gint result_length;
  gchar *result = NULL;
  gchar *p;
  gint i;

  result_length = 0;
  for (i = 0; len < 0 || i < len; i++)
    {
      if (!str[i])
	break;

      if (str[i] >= 0x80000000)
	goto err_out;

      result_length += UTF8_LENGTH (str[i]);
    }

  result = g_malloc (result_length + 1);
  if (!result)
    return NULL;
  p = result;

  i = 0;
  while (p < result + result_length)
    p += g_unichar_to_utf8 (str[i++], p);

  *p = '\0';

  if (items_written)
    *items_written = p - result;

err_out:
  if (items_read)
    *items_read = i;

  return result;
}
