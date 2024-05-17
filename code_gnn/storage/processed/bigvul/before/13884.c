pdf_dict_finds(fz_context *ctx, pdf_obj *obj, const char *key)
{
	int len = DICT(obj)->len;
	if ((obj->flags & PDF_FLAGS_SORTED) && len > 0)
	{
		int l = 0;
		int r = len - 1;

		if (strcmp(pdf_to_name(ctx, DICT(obj)->items[r].k), key) < 0)
		{
			return -1 - (r+1);
		}

		while (l <= r)
		{
			int m = (l + r) >> 1;
			int c = -strcmp(pdf_to_name(ctx, DICT(obj)->items[m].k), key);
			if (c < 0)
				r = m - 1;
			else if (c > 0)
				l = m + 1;
			else
				return m;
		}
		return -1 - l;
	}

	else
	{
		int i;
		for (i = 0; i < len; i++)
			if (strcmp(pdf_to_name(ctx, DICT(obj)->items[i].k), key) == 0)
				return i;

		return -1 - len;
	}
}
