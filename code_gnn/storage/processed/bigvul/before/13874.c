pdf_array_grow(fz_context *ctx, pdf_obj_array *obj)
{
	int i;
	int new_cap = (obj->cap * 3) / 2;

	obj->items = fz_resize_array(ctx, obj->items, new_cap, sizeof(pdf_obj*));
	obj->cap = new_cap;

	for (i = obj->len ; i < obj->cap; i++)
		obj->items[i] = NULL;
}
