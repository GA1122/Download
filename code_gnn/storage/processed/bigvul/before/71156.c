static int ceph_aes_encrypt(const void *key, int key_len,
			    void *dst, size_t *dst_len,
			    const void *src, size_t src_len)
{
	struct scatterlist sg_in[2], prealloc_sg;
	struct sg_table sg_out;
	struct crypto_skcipher *tfm = ceph_crypto_alloc_cipher();
	SKCIPHER_REQUEST_ON_STACK(req, tfm);
	int ret;
	char iv[AES_BLOCK_SIZE];
	size_t zero_padding = (0x10 - (src_len & 0x0f));
	char pad[16];

	if (IS_ERR(tfm))
		return PTR_ERR(tfm);

	memset(pad, zero_padding, zero_padding);

	*dst_len = src_len + zero_padding;

	sg_init_table(sg_in, 2);
	sg_set_buf(&sg_in[0], src, src_len);
	sg_set_buf(&sg_in[1], pad, zero_padding);
	ret = setup_sgtable(&sg_out, &prealloc_sg, dst, *dst_len);
	if (ret)
		goto out_tfm;

	crypto_skcipher_setkey((void *)tfm, key, key_len);
	memcpy(iv, aes_iv, AES_BLOCK_SIZE);

	skcipher_request_set_tfm(req, tfm);
	skcipher_request_set_callback(req, 0, NULL, NULL);
	skcipher_request_set_crypt(req, sg_in, sg_out.sgl,
				   src_len + zero_padding, iv);

	 
	ret = crypto_skcipher_encrypt(req);
	skcipher_request_zero(req);
	if (ret < 0) {
		pr_err("ceph_aes_crypt failed %d\n", ret);
		goto out_sg;
	}
	 

out_sg:
	teardown_sgtable(&sg_out);
out_tfm:
	crypto_free_skcipher(tfm);
	return ret;
}