stringprep_find_character_in_table (uint32_t ucs4,
				    const Stringprep_table_element * table)
{
  ssize_t i;

   

  for (i = 0; table[i].start || table[i].end; i++)
    if (ucs4 >= table[i].start &&
	ucs4 <= (table[i].end ? table[i].end : table[i].start))
      return i;

  return -1;
}
