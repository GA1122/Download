 static int shash_no_setkey(struct crypto_shash *tfm, const u8 *key,
			   unsigned int keylen)
// int shash_no_setkey(struct crypto_shash *tfm, const u8 *key,
// 		    unsigned int keylen)
  {
  	return -ENOSYS;
  }