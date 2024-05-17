static void add_root(fz_context *ctx, pdf_obj *obj, pdf_obj ***roots, int *num_roots, int *max_roots)
{
	if (*num_roots == *max_roots)
	{
		int new_max_roots = *max_roots * 2;
		if (new_max_roots == 0)
			new_max_roots = 4;
		*roots = fz_resize_array(ctx, *roots, new_max_roots, sizeof(**roots));
		*max_roots = new_max_roots;
	}
	(*roots)[(*num_roots)++] = pdf_keep_obj(ctx, obj);
}
