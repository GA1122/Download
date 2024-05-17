int get_tmpname(char *fnametmp, const char *fname, BOOL make_unique)
{
	int maxname, length = 0;
	const char *f;
	char *suf;

	if (tmpdir) {
		 
		length = strlcpy(fnametmp, tmpdir, MAXPATHLEN - 2);
		fnametmp[length++] = '/';
	}

	if ((f = strrchr(fname, '/')) != NULL) {
		++f;
		if (!tmpdir) {
			length = f - fname;
			 
			strlcpy(fnametmp, fname, length + 1);
		}
	} else
		f = fname;

	if (!tmpdir) {  
		if (*f == '.')  
			f++;
		fnametmp[length++] = '.';
	}

	 
	maxname = MIN(MAXPATHLEN - length - TMPNAME_SUFFIX_LEN,
		      NAME_MAX - 1 - TMPNAME_SUFFIX_LEN);

	if (maxname < 0) {
		rprintf(FERROR_XFER, "temporary filename too long: %s\n", fname);
		fnametmp[0] = '\0';
		return 0;
	}

	if (maxname) {
		int added = strlcpy(fnametmp + length, f, maxname);
		if (added >= maxname)
			added = maxname - 1;
		suf = fnametmp + length + added;

		 
		if ((int)f[added] & 0x80) {
			while ((int)suf[-1] & 0x80)
				suf--;
		}
		 
		if (suf[-1] == '.')
			suf--;
	} else
		suf = fnametmp + length - 1;  

	if (make_unique) {
		static unsigned counter_limit;
		unsigned counter;

		if (!counter_limit) {
			counter_limit = (unsigned)getpid() + MAX_UNIQUE_LOOP;
			if (counter_limit > MAX_UNIQUE_NUMBER || counter_limit < MAX_UNIQUE_LOOP)
				counter_limit = MAX_UNIQUE_LOOP;
		}
		counter = counter_limit - MAX_UNIQUE_LOOP;

		 
		while (1) {
			snprintf(suf, TMPNAME_SUFFIX_LEN+1, ".%d", counter);
			if (access(fnametmp, 0) < 0)
				break;
			if (++counter >= counter_limit)
				return 0;
		}
	} else
		memcpy(suf, TMPNAME_SUFFIX, TMPNAME_SUFFIX_LEN+1);

	return 1;
}
