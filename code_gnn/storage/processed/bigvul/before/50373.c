struct posix_acl *get_acl(struct inode *inode, int type)
{
	void *sentinel;
	struct posix_acl **p;
	struct posix_acl *acl;

	 

	acl = get_cached_acl(inode, type);
	if (!is_uncached_acl(acl))
		return acl;

	if (!IS_POSIXACL(inode))
		return NULL;

	sentinel = uncached_acl_sentinel(current);
	p = acl_by_type(inode, type);

	 
	if (cmpxchg(p, ACL_NOT_CACHED, sentinel) != ACL_NOT_CACHED)
		  ;

	 
	if (!inode->i_op->get_acl) {
		set_cached_acl(inode, type, NULL);
		return NULL;
	}
	acl = inode->i_op->get_acl(inode, type);

	if (IS_ERR(acl)) {
		 
		cmpxchg(p, sentinel, ACL_NOT_CACHED);
		return acl;
	}

	 
	posix_acl_dup(acl);
	if (unlikely(cmpxchg(p, sentinel, acl) != sentinel))
		posix_acl_release(acl);
	return acl;
}
