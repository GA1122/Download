int fscrypt_fname_alloc_buffer(const struct inode *inode,
				u32 ilen, struct fscrypt_str *crypto_str)
{
	unsigned int olen = fscrypt_fname_encrypted_size(inode, ilen);

	crypto_str->len = olen;
	if (olen < FS_FNAME_CRYPTO_DIGEST_SIZE * 2)
		olen = FS_FNAME_CRYPTO_DIGEST_SIZE * 2;
	 
	crypto_str->name = kmalloc(olen + 1, GFP_NOFS);
	if (!(crypto_str->name))
		return -ENOMEM;
	return 0;
}
