static __be32 nfsd4_encode_path(struct xdr_stream *xdr,
				const struct path *root,
				const struct path *path)
{
	struct path cur = *path;
	__be32 *p;
	struct dentry **components = NULL;
	unsigned int ncomponents = 0;
	__be32 err = nfserr_jukebox;

	dprintk("nfsd4_encode_components(");

	path_get(&cur);
	 
	for (;;) {
		if (path_equal(&cur, root))
			break;
		if (cur.dentry == cur.mnt->mnt_root) {
			if (follow_up(&cur))
				continue;
			goto out_free;
		}
		if ((ncomponents & 15) == 0) {
			struct dentry **new;
			new = krealloc(components,
					sizeof(*new) * (ncomponents + 16),
					GFP_KERNEL);
			if (!new)
				goto out_free;
			components = new;
		}
		components[ncomponents++] = cur.dentry;
		cur.dentry = dget_parent(cur.dentry);
	}
	err = nfserr_resource;
	p = xdr_reserve_space(xdr, 4);
	if (!p)
		goto out_free;
	*p++ = cpu_to_be32(ncomponents);

	while (ncomponents) {
		struct dentry *dentry = components[ncomponents - 1];
		unsigned int len;

		spin_lock(&dentry->d_lock);
		len = dentry->d_name.len;
		p = xdr_reserve_space(xdr, len + 4);
		if (!p) {
			spin_unlock(&dentry->d_lock);
			goto out_free;
		}
		p = xdr_encode_opaque(p, dentry->d_name.name, len);
		dprintk("/%pd", dentry);
		spin_unlock(&dentry->d_lock);
		dput(dentry);
		ncomponents--;
	}

	err = 0;
out_free:
	dprintk(")\n");
	while (ncomponents)
		dput(components[--ncomponents]);
	kfree(components);
	path_put(&cur);
	return err;
}