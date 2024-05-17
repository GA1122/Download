static int ccm_encrypt(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_aes_ctx *ctx = crypto_aead_ctx(aead);
	struct blkcipher_desc desc = { .info = req->iv };
	struct blkcipher_walk walk;
	u8 __aligned(8) mac[AES_BLOCK_SIZE];
	u8 buf[AES_BLOCK_SIZE];
	u32 len = req->cryptlen;
	int err;

	err = ccm_init_mac(req, mac, len);
	if (err)
		return err;

	kernel_neon_begin_partial(6);

	if (req->assoclen)
		ccm_calculate_auth_mac(req, mac);

	 
	memcpy(buf, req->iv, AES_BLOCK_SIZE);

	blkcipher_walk_init(&walk, req->dst, req->src, len);
	err = blkcipher_aead_walk_virt_block(&desc, &walk, aead,
					     AES_BLOCK_SIZE);

	while (walk.nbytes) {
		u32 tail = walk.nbytes % AES_BLOCK_SIZE;

		if (walk.nbytes == len)
			tail = 0;

		ce_aes_ccm_encrypt(walk.dst.virt.addr, walk.src.virt.addr,
				   walk.nbytes - tail, ctx->key_enc,
				   num_rounds(ctx), mac, walk.iv);

		len -= walk.nbytes - tail;
		err = blkcipher_walk_done(&desc, &walk, tail);
	}
	if (!err)
		ce_aes_ccm_final(mac, buf, ctx->key_enc, num_rounds(ctx));

	kernel_neon_end();

	if (err)
		return err;

	 
	scatterwalk_map_and_copy(mac, req->dst, req->cryptlen,
				 crypto_aead_authsize(aead), 1);

	return 0;
}