stringprep_4zi (uint32_t * ucs4, size_t maxucs4len,
		Stringprep_profile_flags flags,
		const Stringprep_profile * profile)
{
  size_t ucs4len;

  for (ucs4len = 0; ucs4len < maxucs4len && ucs4[ucs4len] != 0; ucs4len++)
    ;

  return stringprep_4zi_1 (ucs4, ucs4len, maxucs4len, flags, profile);
}
