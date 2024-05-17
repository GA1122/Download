stringprep_ucs4_to_utf8 (const uint32_t * str, ssize_t len,
			 size_t * items_read, size_t * items_written)
{
  return g_ucs4_to_utf8 (str, len, (glong *) items_read,
			 (glong *) items_written);
}
