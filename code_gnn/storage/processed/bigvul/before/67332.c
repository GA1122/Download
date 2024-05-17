static inline int dentry_cmp(const struct dentry *dentry, const unsigned char *ct, unsigned tcount)
{
	 
	const unsigned char *cs = lockless_dereference(dentry->d_name.name);

	return dentry_string_cmp(cs, ct, tcount);
}
