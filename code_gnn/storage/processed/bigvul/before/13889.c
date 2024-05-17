pdf_dict_getp(fz_context *ctx, pdf_obj *obj, const char *keys)
{
	char buf[256];
	char *k, *e;

	RESOLVE(obj);
	if (!OBJ_IS_DICT(obj))
		return NULL;
	if (strlen(keys)+1 > 256)
		fz_throw(ctx, FZ_ERROR_GENERIC, "path too long");

	strcpy(buf, keys);

	e = buf;
	while (*e && obj)
	{
		k = e;
		while (*e != '/' && *e != '\0')
			e++;

		if (*e == '/')
		{
			*e = '\0';
			e++;
		}

		obj = pdf_dict_gets(ctx, obj, k);
	}

	return obj;
}
