static struct dentry *ovl_upperdentry_dereference(struct ovl_entry *oe)
{
	struct dentry *upperdentry = ACCESS_ONCE(oe->__upperdentry);
	 
	smp_read_barrier_depends();
	return upperdentry;
}
