static int ceph_aes_decrypt(const void *key, int key_len,
			    void *dst, size_t *dst_len,
			    const void *src, size_t src_len)
{
	struct sg_table sg_in;
	struct scatterlist sg_out[2], prealloc_sg;
	struct crypto_skcipher *tfm = ceph_crypto_alloc_cipher();
	SKCIPHER_REQUEST_ON_STACK(req, tfm);
	char pad[16];
	char iv[AES_BLOCK_SIZE];
	int ret;
	int last_byte;

	if (IS_ERR(tfm))
		return PTR_ERR(tfm);

	sg_init_table(sg_out, 2);
	sg_set_buf(&sg_out[0], dst, *dst_len);
	sg_set_buf(&sg_out[1], pad, sizeof(pad));
	ret = setup_sgtable(&sg_in, &prealloc_sg, src, src_len);
	if (ret)
		goto out_tfm;

	crypto_skcipher_setkey((void *)tfm, key, key_len);
	memcpy(iv, aes_iv, AES_BLOCK_SIZE);

	skcipher_request_set_tfm(req, tfm);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg_in.sgl, sg_out,
				   src_len, iv);

	 
	ret = crypto_skcipher_decrypt(req);
	skcipher_request_zero(req);
	if (ret < 0) {
		pr_err("ceph_aes_decrypt failed %d\n", ret);
		goto out_sg;
	}

	if (src_len <= *dst_len)
		last_byte = ((char *)dst)[src_len - 1];
	else
		last_byte = pad[src_len - *dst_len - 1];
	if (last_byte <= 16 && src_len >= last_byte) {
		*dst_len = src_len - last_byte;
	} else {
		pr_err("ceph_aes_decrypt got bad padding %d on src len %d\n",
		       last_byte, (int)src_len);
		return -EPERM;   
	}
	 

out_sg:
	teardown_sgtable(&sg_in);
out_tfm:
	crypto_free_skcipher(tfm);
	return ret;
}