static int fname_encrypt(struct inode *inode,
			const struct qstr *iname, struct fscrypt_str *oname)
{
	struct skcipher_request *req = NULL;
	DECLARE_FS_COMPLETION_RESULT(ecr);
	struct fscrypt_info *ci = inode->i_crypt_info;
	struct crypto_skcipher *tfm = ci->ci_ctfm;
	int res = 0;
	char iv[FS_CRYPTO_BLOCK_SIZE];
	struct scatterlist sg;
	int padding = 4 << (ci->ci_flags & FS_POLICY_FLAGS_PAD_MASK);
	unsigned int lim;
	unsigned int cryptlen;

	lim = inode->i_sb->s_cop->max_namelen(inode);
	if (iname->len <= 0 || iname->len > lim)
		return -EIO;

	 
	cryptlen = max_t(unsigned int, iname->len, FS_CRYPTO_BLOCK_SIZE);
	cryptlen = round_up(cryptlen, padding);
	cryptlen = min(cryptlen, lim);
	memcpy(oname->name, iname->name, iname->len);
	memset(oname->name + iname->len, 0, cryptlen - iname->len);

	 
	memset(iv, 0, FS_CRYPTO_BLOCK_SIZE);

	 
	req = skcipher_request_alloc(tfm, GFP_NOFS);
	if (!req) {
		printk_ratelimited(KERN_ERR
			"%s: skcipher_request_alloc() failed\n", __func__);
		return -ENOMEM;
	}
	skcipher_request_set_callback(req,
			CRYPTO_TFM_REQ_MAY_BACKLOG | CRYPTO_TFM_REQ_MAY_SLEEP,
			fname_crypt_complete, &ecr);
	sg_init_one(&sg, oname->name, cryptlen);
	skcipher_request_set_crypt(req, &sg, &sg, cryptlen, iv);

	 
	res = crypto_skcipher_encrypt(req);
	if (res == -EINPROGRESS || res == -EBUSY) {
		 
		wait_for_completion(&ecr.completion);
		res = ecr.res;
	}
	skcipher_request_free(req);
	if (res < 0) {
		printk_ratelimited(KERN_ERR
				"%s: Error (error code %d)\n", __func__, res);
		return res;
	}

	oname->len = cryptlen;
	return 0;
}