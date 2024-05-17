int big_key_preparse(struct key_preparsed_payload *prep)
{
	struct path *path = (struct path *)&prep->payload.data[big_key_path];
	struct file *file;
	u8 *enckey;
	u8 *data = NULL;
	ssize_t written;
	size_t datalen = prep->datalen;
	int ret;

	ret = -EINVAL;
	if (datalen <= 0 || datalen > 1024 * 1024 || !prep->data)
		goto error;

	 
	prep->quotalen = 16;

	prep->payload.data[big_key_len] = (void *)(unsigned long)datalen;

	if (datalen > BIG_KEY_FILE_THRESHOLD) {
		 
		size_t enclen = ALIGN(datalen, crypto_skcipher_blocksize(big_key_skcipher));

		 
		data = kmalloc(enclen, GFP_KERNEL);
		if (!data)
			return -ENOMEM;

		memcpy(data, prep->data, datalen);
		memset(data + datalen, 0x00, enclen - datalen);

		 
		enckey = kmalloc(ENC_KEY_SIZE, GFP_KERNEL);
		if (!enckey) {
			ret = -ENOMEM;
			goto error;
		}

		ret = big_key_gen_enckey(enckey);
		if (ret)
			goto err_enckey;

		 
		ret = big_key_crypt(BIG_KEY_ENC, data, enclen, enckey);
		if (ret)
			goto err_enckey;

		 
		file = shmem_kernel_file_setup("", enclen, 0);
		if (IS_ERR(file)) {
			ret = PTR_ERR(file);
			goto err_enckey;
		}

		written = kernel_write(file, data, enclen, 0);
		if (written != enclen) {
			ret = written;
			if (written >= 0)
				ret = -ENOMEM;
			goto err_fput;
		}

		 
		prep->payload.data[big_key_data] = enckey;
		*path = file->f_path;
		path_get(path);
		fput(file);
		kfree(data);
	} else {
		 
		void *data = kmalloc(datalen, GFP_KERNEL);

		if (!data)
			return -ENOMEM;

		prep->payload.data[big_key_data] = data;
		memcpy(data, prep->data, prep->datalen);
	}
	return 0;

err_fput:
	fput(file);
err_enckey:
	kfree(enckey);
error:
	kfree(data);
	return ret;
}