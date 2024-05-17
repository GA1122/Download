 int fscrypt_setup_filename(struct inode *dir, const struct qstr *iname,
 			      int lookup, struct fscrypt_name *fname)
 {
 	int ret = 0, bigname = 0;
 
 	memset(fname, 0, sizeof(struct fscrypt_name));
 	fname->usr_fname = iname;
 
 	if (!dir->i_sb->s_cop->is_encrypted(dir) ||
 				fscrypt_is_dot_dotdot(iname)) {
 		fname->disk_name.name = (unsigned char *)iname->name;
  		fname->disk_name.len = iname->len;
  		return 0;
  	}
	ret = fscrypt_get_crypt_info(dir);
// 	ret = fscrypt_get_encryption_info(dir);
  	if (ret && ret != -EOPNOTSUPP)
  		return ret;
  
 	if (dir->i_crypt_info) {
 		ret = fscrypt_fname_alloc_buffer(dir, iname->len,
 							&fname->crypto_buf);
 		if (ret)
 			return ret;
 		ret = fname_encrypt(dir, iname, &fname->crypto_buf);
 		if (ret)
 			goto errout;
 		fname->disk_name.name = fname->crypto_buf.name;
 		fname->disk_name.len = fname->crypto_buf.len;
 		return 0;
 	}
 	if (!lookup)
 		return -ENOKEY;
 
 	 
 	if (iname->name[0] == '_')
 		bigname = 1;
 	if ((bigname && (iname->len != 33)) || (!bigname && (iname->len > 43)))
 		return -ENOENT;
 
 	fname->crypto_buf.name = kmalloc(32, GFP_KERNEL);
 	if (fname->crypto_buf.name == NULL)
 		return -ENOMEM;
 
 	ret = digest_decode(iname->name + bigname, iname->len - bigname,
 				fname->crypto_buf.name);
 	if (ret < 0) {
 		ret = -ENOENT;
 		goto errout;
 	}
 	fname->crypto_buf.len = ret;
 	if (bigname) {
 		memcpy(&fname->hash, fname->crypto_buf.name, 4);
 		memcpy(&fname->minor_hash, fname->crypto_buf.name + 4, 4);
 	} else {
 		fname->disk_name.name = fname->crypto_buf.name;
 		fname->disk_name.len = fname->crypto_buf.len;
 	}
 	return 0;
 
 errout:
 	fscrypt_fname_free_buffer(&fname->crypto_buf);
 	return ret;
 }