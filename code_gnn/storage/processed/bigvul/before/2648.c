setPathArray(JsonbIterator **it, Datum *path_elems, bool *path_nulls,
			 int path_len, JsonbParseState **st, int level,
			 Jsonb *newval, uint32 nelems, bool create)
{
	JsonbValue	v;
	int			idx,
				i;
	char	   *badp;
	bool		done = false;

	 
	if (level < path_len && !path_nulls[level])
	{
		char	   *c = VARDATA_ANY(path_elems[level]);
		long		lindex;

		errno = 0;
		lindex = strtol(c, &badp, 10);
		if (errno != 0 || badp == c || *badp != '\0' || lindex > INT_MAX ||
			lindex < INT_MIN)
			elog(ERROR, "path element at the position %d is not an integer", level + 1);
		else
			idx = lindex;
	}
	else
		idx = nelems;

	if (idx < 0)
	{
		if (-idx > nelems)
			idx = INT_MIN;
		else
			idx = nelems + idx;
	}

	if (idx > 0 && idx > nelems)
		idx = nelems;

	 

	if ((idx == INT_MIN || nelems == 0) && create && (level == path_len - 1))
	{
		Assert(newval != NULL);
		addJsonbToParseState(st, newval);
		done = true;
	}

	 
	for (i = 0; i < nelems; i++)
	{
		int			r;

		if (i == idx && level < path_len)
		{
			if (level == path_len - 1)
			{
				r = JsonbIteratorNext(it, &v, true);	 
				if (newval != NULL)
					addJsonbToParseState(st, newval);

				done = true;
			}
			else
				(void) setPath(it, path_elems, path_nulls, path_len,
							   st, level + 1, newval, create);
		}
		else
		{
			r = JsonbIteratorNext(it, &v, false);

			(void) pushJsonbValue(st, r, r < WJB_BEGIN_ARRAY ? &v : NULL);

			if (r == WJB_BEGIN_ARRAY || r == WJB_BEGIN_OBJECT)
			{
				int			walking_level = 1;

				while (walking_level != 0)
				{
					r = JsonbIteratorNext(it, &v, false);

					if (r == WJB_BEGIN_ARRAY || r == WJB_BEGIN_OBJECT)
						++walking_level;
					if (r == WJB_END_ARRAY || r == WJB_END_OBJECT)
						--walking_level;

					(void) pushJsonbValue(st, r, r < WJB_BEGIN_ARRAY ? &v : NULL);
				}
			}

			if (create && !done && level == path_len - 1 && i == nelems - 1)
			{
				addJsonbToParseState(st, newval);
			}

		}
	}
}
