static int path_lookupat(int dfd, const struct filename *name,
				unsigned int flags, struct nameidata *nd)
{
	struct path path;
	int err;

	 
	err = path_init(dfd, name, flags, nd);
	if (!err && !(flags & LOOKUP_PARENT)) {
		err = lookup_last(nd, &path);
		while (err > 0) {
			void *cookie;
			struct path link = path;
			err = may_follow_link(&link, nd);
			if (unlikely(err))
				break;
			nd->flags |= LOOKUP_PARENT;
			err = follow_link(&link, nd, &cookie);
			if (err)
				break;
			err = lookup_last(nd, &path);
			put_link(nd, &link, cookie);
		}
	}

	if (!err)
		err = complete_walk(nd);

	if (!err && nd->flags & LOOKUP_DIRECTORY) {
		if (!d_can_lookup(nd->path.dentry)) {
			path_put(&nd->path);
			err = -ENOTDIR;
		}
	}

	path_cleanup(nd);
	return err;
}