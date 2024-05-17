pdf_dict_getl(fz_context *ctx, pdf_obj *obj, ...)
{
	va_list keys;
	pdf_obj *key;

	va_start(keys, obj);

	while (obj != NULL && (key = va_arg(keys, pdf_obj *)) != NULL)
	{
		obj = pdf_dict_get(ctx, obj, key);
	}

	va_end(keys);
	return obj;
}
