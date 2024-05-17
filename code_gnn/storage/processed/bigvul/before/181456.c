 static int validate_user_key(struct fscrypt_info *crypt_info,
 			struct fscrypt_context *ctx, u8 *raw_key,
 			const char *prefix)
 {
 	char *description;
 	struct key *keyring_key;
 	struct fscrypt_key *master_key;
 	const struct user_key_payload *ukp;
 	int res;
 
 	description = kasprintf(GFP_NOFS, "%s%*phN", prefix,
 				FS_KEY_DESCRIPTOR_SIZE,
 				ctx->master_key_descriptor);
 	if (!description)
 		return -ENOMEM;
 
 	keyring_key = request_key(&key_type_logon, description, NULL);
  	kfree(description);
  	if (IS_ERR(keyring_key))
  		return PTR_ERR(keyring_key);
// 	down_read(&keyring_key->sem);
  
  	if (keyring_key->type != &key_type_logon) {
  		printk_once(KERN_WARNING
  				"%s: key type must be logon\n", __func__);
  		res = -ENOKEY;
  		goto out;
  	}
	down_read(&keyring_key->sem);
  	ukp = user_key_payload(keyring_key);
  	if (ukp->datalen != sizeof(struct fscrypt_key)) {
  		res = -EINVAL;
		up_read(&keyring_key->sem);
  		goto out;
  	}
  	master_key = (struct fscrypt_key *)ukp->data;
 	BUILD_BUG_ON(FS_AES_128_ECB_KEY_SIZE != FS_KEY_DERIVATION_NONCE_SIZE);
 
 	if (master_key->size != FS_AES_256_XTS_KEY_SIZE) {
 		printk_once(KERN_WARNING
  				"%s: key size incorrect: %d\n",
  				__func__, master_key->size);
  		res = -ENOKEY;
		up_read(&keyring_key->sem);
  		goto out;
  	}
  	res = derive_key_aes(ctx->nonce, master_key->raw, raw_key);
	up_read(&keyring_key->sem);
	if (res)
		goto out;
	crypt_info->ci_keyring_key = keyring_key;
	return 0;
  out:
// 	up_read(&keyring_key->sem);
  	key_put(keyring_key);
  	return res;
  }