static struct dentry *ovl_check_empty_and_clear(struct dentry *dentry)
{
	int err;
	struct dentry *ret = NULL;
	LIST_HEAD(list);

	err = ovl_check_empty_dir(dentry, &list);
	if (err)
		ret = ERR_PTR(err);
	else {
		 
		if (ovl_dentry_upper(dentry))
			ret = ovl_clear_empty(dentry, &list);
	}

	ovl_cache_free(&list);

	return ret;
}
