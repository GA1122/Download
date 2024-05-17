pdf_array_find(fz_context *ctx, pdf_obj *arr, pdf_obj *obj)
{
	int i, len;

	len = pdf_array_len(ctx, arr);
	for (i = 0; i < len; i++)
		if (!pdf_objcmp(ctx, pdf_array_get(ctx, arr, i), obj))
			return i;

	return -1;
}
