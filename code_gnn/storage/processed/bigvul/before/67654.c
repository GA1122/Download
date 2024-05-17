int fscrypt_fname_usr_to_disk(struct inode *inode,
			const struct qstr *iname,
			struct fscrypt_str *oname)
{
	if (fscrypt_is_dot_dotdot(iname)) {
		oname->name[0] = '.';
		oname->name[iname->len - 1] = '.';
		oname->len = iname->len;
		return 0;
	}
	if (inode->i_crypt_info)
		return fname_encrypt(inode, iname, oname);
	 
	return -ENOKEY;
}