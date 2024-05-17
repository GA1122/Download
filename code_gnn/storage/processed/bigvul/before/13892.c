pdf_dict_grow(fz_context *ctx, pdf_obj *obj)
{
	int i;
	int new_cap = (DICT(obj)->cap * 3) / 2;

	DICT(obj)->items = fz_resize_array(ctx, DICT(obj)->items, new_cap, sizeof(struct keyval));
	DICT(obj)->cap = new_cap;

	for (i = DICT(obj)->len; i < DICT(obj)->cap; i++)
	{
		DICT(obj)->items[i].k = NULL;
		DICT(obj)->items[i].v = NULL;
	}
}
