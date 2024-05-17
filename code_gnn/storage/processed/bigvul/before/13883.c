pdf_dict_find(fz_context *ctx, pdf_obj *obj, pdf_obj *key)
{
	int len = DICT(obj)->len;
	if ((obj->flags & PDF_FLAGS_SORTED) && len > 0)
	{
		int l = 0;
		int r = len - 1;
		pdf_obj *k = DICT(obj)->items[r].k;

		if (k == key || (k >= PDF_OBJ__LIMIT && strcmp(NAME(k)->n, PDF_NAMES[(intptr_t)key]) < 0))
		{
			return -1 - (r+1);
		}

		while (l <= r)
		{
			int m = (l + r) >> 1;
			int c;

			k = DICT(obj)->items[m].k;
			c = (k < PDF_OBJ__LIMIT ? (char *)key-(char *)k : -strcmp(NAME(k)->n, PDF_NAMES[(intptr_t)key]));
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
		{
			pdf_obj *k = DICT(obj)->items[i].k;
			if (k < PDF_OBJ__LIMIT)
			{
				if (k == key)
					return i;
			}
			else
			{
				if (!strcmp(PDF_NAMES[(intptr_t)key], NAME(k)->n))
					return i;
			}
		}

		return -1 - len;
	}
}
