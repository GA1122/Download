 void fscrypt_put_encryption_info(struct inode *inode, struct fscrypt_info *ci)
 {
	struct fscrypt_info *prev;

	if (ci == NULL)
		ci = ACCESS_ONCE(inode->i_crypt_info);
	if (ci == NULL)
		return;

	prev = cmpxchg(&inode->i_crypt_info, ci, NULL);
	if (prev != ci)
		return;

 	put_crypt_info(ci);
 }