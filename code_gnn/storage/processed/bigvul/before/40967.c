json_bool lh_table_lookup_ex(struct lh_table* t, const void* k, void **v)
{
	struct lh_entry *e = lh_table_lookup_entry(t, k);
	if (e != NULL) {
		if (v != NULL) *v = (void *)e->v;
		return TRUE;  
	}
	if (v != NULL) *v = NULL;
	return FALSE;  
}
