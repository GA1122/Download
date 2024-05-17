struct dentry *__d_alloc(struct super_block *sb, const struct qstr *name)
{
	struct dentry *dentry;
	char *dname;

	dentry = kmem_cache_alloc(dentry_cache, GFP_KERNEL);
	if (!dentry)
		return NULL;

	 
	dentry->d_iname[DNAME_INLINE_LEN-1] = 0;
	if (name->len > DNAME_INLINE_LEN-1) {
		size_t size = offsetof(struct external_name, name[1]);
		struct external_name *p = kmalloc(size + name->len, GFP_KERNEL);
		if (!p) {
			kmem_cache_free(dentry_cache, dentry); 
			return NULL;
		}
		atomic_set(&p->u.count, 1);
		dname = p->name;
		if (IS_ENABLED(CONFIG_DCACHE_WORD_ACCESS))
			kasan_unpoison_shadow(dname,
				round_up(name->len + 1,	sizeof(unsigned long)));
	} else  {
		dname = dentry->d_iname;
	}	

	dentry->d_name.len = name->len;
	dentry->d_name.hash = name->hash;
	memcpy(dname, name->name, name->len);
	dname[name->len] = 0;

	 
	smp_wmb();
	dentry->d_name.name = dname;

	dentry->d_lockref.count = 1;
	dentry->d_flags = 0;
	spin_lock_init(&dentry->d_lock);
	seqcount_init(&dentry->d_seq);
	dentry->d_inode = NULL;
	dentry->d_parent = dentry;
	dentry->d_sb = sb;
	dentry->d_op = NULL;
	dentry->d_fsdata = NULL;
	INIT_HLIST_BL_NODE(&dentry->d_hash);
	INIT_LIST_HEAD(&dentry->d_lru);
	INIT_LIST_HEAD(&dentry->d_subdirs);
	INIT_HLIST_NODE(&dentry->d_u.d_alias);
	INIT_LIST_HEAD(&dentry->d_child);
	d_set_d_op(dentry, dentry->d_sb->s_d_op);

	this_cpu_inc(nr_dentry);

	return dentry;
}