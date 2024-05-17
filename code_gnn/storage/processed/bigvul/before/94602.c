static inline int dentry_cmp(const struct dentry *dentry, const unsigned char *ct, unsigned tcount)
{
	const unsigned char *cs;
	 
	cs = ACCESS_ONCE(dentry->d_name.name);
	smp_read_barrier_depends();
	return dentry_string_cmp(cs, ct, tcount);
}
