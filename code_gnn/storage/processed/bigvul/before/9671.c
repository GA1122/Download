stringprep_unichar_to_utf8 (uint32_t c, char *outbuf)
{
  return g_unichar_to_utf8 (c, outbuf);
}
