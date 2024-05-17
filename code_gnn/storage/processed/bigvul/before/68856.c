static void check_poison_obj(struct kmem_cache *cachep, void *objp)
{
	char *realobj;
	int size, i;
	int lines = 0;

	if (is_debug_pagealloc_cache(cachep))
		return;

	realobj = (char *)objp + obj_offset(cachep);
	size = cachep->object_size;

	for (i = 0; i < size; i++) {
		char exp = POISON_FREE;
		if (i == size - 1)
			exp = POISON_END;
		if (realobj[i] != exp) {
			int limit;
			 
			 
			if (lines == 0) {
				pr_err("Slab corruption (%s): %s start=%p, len=%d\n",
				       print_tainted(), cachep->name,
				       realobj, size);
				print_objinfo(cachep, objp, 0);
			}
			 
			i = (i / 16) * 16;
			limit = 16;
			if (i + limit > size)
				limit = size - i;
			dump_line(realobj, i, limit);
			i += 16;
			lines++;
			 
			if (lines > 5)
				break;
		}
	}
	if (lines != 0) {
		 
		struct page *page = virt_to_head_page(objp);
		unsigned int objnr;

		objnr = obj_to_index(cachep, page, objp);
		if (objnr) {
			objp = index_to_obj(cachep, page, objnr - 1);
			realobj = (char *)objp + obj_offset(cachep);
			pr_err("Prev obj: start=%p, len=%d\n", realobj, size);
			print_objinfo(cachep, objp, 2);
		}
		if (objnr + 1 < cachep->num) {
			objp = index_to_obj(cachep, page, objnr + 1);
			realobj = (char *)objp + obj_offset(cachep);
			pr_err("Next obj: start=%p, len=%d\n", realobj, size);
			print_objinfo(cachep, objp, 2);
		}
	}
}