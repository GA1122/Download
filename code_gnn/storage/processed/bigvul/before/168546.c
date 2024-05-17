static unsigned long htab_hash(char* str)
{
	unsigned long hval, hval2;
	unsigned long idx;
	unsigned long r = 5381;
	int c;
	char* sz = str;

	if (str == NULL)
		return 0;

	while ((c = *sz++) != 0)
		r = ((r << 5) + r) + c;
	if (r == 0)
		++r;

	hval = r % htab_size;
	if (hval == 0)
		++hval;

	idx = hval;

	if (htab_table[idx].used) {
		if ( (htab_table[idx].used == hval)
		  && (safe_strcmp(str, htab_table[idx].str) == 0) ) {
			return idx;
		}
		usbi_dbg("hash collision ('%s' vs '%s')", str, htab_table[idx].str);

		hval2 = 1 + hval % (htab_size - 2);

		do {
			if (idx <= hval2) {
				idx = htab_size + idx - hval2;
			} else {
				idx -= hval2;
			}

			if (idx == hval) {
				break;
			}

			if ( (htab_table[idx].used == hval)
			  && (safe_strcmp(str, htab_table[idx].str) == 0) ) {
				return idx;
			}
		}
		while (htab_table[idx].used);
	}


	if (htab_filled >= htab_size) {
		usbi_err(NULL, "hash table is full (%d entries)", htab_size);
		return 0;
	}

	usbi_mutex_lock(&htab_write_mutex);
	safe_free(htab_table[idx].str);
	htab_table[idx].used = hval;
	htab_table[idx].str = (char*) malloc(safe_strlen(str)+1);
	if (htab_table[idx].str == NULL) {
		usbi_err(NULL, "could not duplicate string for hash table");
		usbi_mutex_unlock(&htab_write_mutex);
		return 0;
	}
	memcpy(htab_table[idx].str, str, safe_strlen(str)+1);
	++htab_filled;
	usbi_mutex_unlock(&htab_write_mutex);

	return idx;
}
