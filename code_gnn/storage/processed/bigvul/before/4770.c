g_utf8_normalize (const gchar * str, gssize len, GNormalizeMode mode)
{
  gunichar *result_wc = _g_utf8_normalize_wc (str, len, mode);
  gchar *result;

  result = g_ucs4_to_utf8 (result_wc, -1, NULL, NULL);
  g_free (result_wc);

  return result;
}
