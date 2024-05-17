struct dentry *d_hash_and_lookup(struct dentry *dir, struct qstr *name)
{
	 
	name->hash = full_name_hash(name->name, name->len);
	if (dir->d_flags & DCACHE_OP_HASH) {
		int err = dir->d_op->d_hash(dir, name);
		if (unlikely(err < 0))
			return ERR_PTR(err);
	}
	return d_lookup(dir, name);
}
