g_utf8_get_char (const gchar * p)
{
  int i, mask = 0, len;
  gunichar result;
  unsigned char c = (unsigned char) *p;

  UTF8_COMPUTE (c, mask, len);
  if (len == -1)
    return (gunichar) - 1;
  UTF8_GET (result, p, i, mask, len);

  return result;
}
