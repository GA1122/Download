stringprep_apply_table_to_string (uint32_t * ucs4,
				  size_t * ucs4len,
				  size_t maxucs4len,
				  const Stringprep_table_element * table)
{
  ssize_t pos;
  size_t i, maplen;

  while ((pos = stringprep_find_string_in_table (ucs4, *ucs4len,
						 &i, table)) != -1)
    {
      for (maplen = STRINGPREP_MAX_MAP_CHARS;
	   maplen > 0 && table[i].map[maplen - 1] == 0; maplen--)
	;

      if (*ucs4len - 1 + maplen >= maxucs4len)
	return STRINGPREP_TOO_SMALL_BUFFER;

      memmove (&ucs4[pos + maplen], &ucs4[pos + 1],
	       sizeof (uint32_t) * (*ucs4len - pos - 1));
      memcpy (&ucs4[pos], table[i].map, sizeof (uint32_t) * maplen);
      *ucs4len = *ucs4len - 1 + maplen;
    }

  return STRINGPREP_OK;
}
