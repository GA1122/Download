static int add_ref_tag(const char *path, const struct object_id *oid, int flag, void *cb_data)
{
	struct object_id peeled;

	if (starts_with(path, "refs/tags/") &&  
	    !peel_ref(path, peeled.hash)    &&  
	    packlist_find(&to_pack, peeled.hash, NULL))       
		add_object_entry(oid->hash, OBJ_TAG, NULL, 0);
	return 0;
}
