sshkey_load_public(const char *filename, struct sshkey **keyp, char **commentp)
{
	struct sshkey *pub = NULL;
	char file[PATH_MAX];
	int r, fd;

	if (keyp != NULL)
		*keyp = NULL;
	if (commentp != NULL)
		*commentp = NULL;

	 

	if ((fd = open(filename, O_RDONLY)) < 0)
		goto skip;
#ifdef WITH_SSH1
	 
	r = sshkey_load_public_rsa1(fd, keyp, commentp);
	close(fd);
	switch (r) {
	case SSH_ERR_INTERNAL_ERROR:
	case SSH_ERR_ALLOC_FAIL:
	case SSH_ERR_INVALID_ARGUMENT:
	case SSH_ERR_SYSTEM_ERROR:
	case 0:
		return r;
	}
#else  
	close(fd);
#endif  

	 
	if ((pub = sshkey_new(KEY_UNSPEC)) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	if ((r = sshkey_try_load_public(pub, filename, commentp)) == 0) {
		if (keyp != NULL)
			*keyp = pub;
		return 0;
	}
	sshkey_free(pub);

#ifdef WITH_SSH1
	 
	if ((pub = sshkey_new(KEY_RSA1)) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	if ((r = sshkey_try_load_public(pub, filename, commentp)) == 0) {
		if (keyp != NULL)
			*keyp = pub;
		return 0;
	}
	sshkey_free(pub);
#endif  

 skip:
	 
	if ((pub = sshkey_new(KEY_UNSPEC)) == NULL)
		return SSH_ERR_ALLOC_FAIL;
	r = SSH_ERR_ALLOC_FAIL;	 
	if ((strlcpy(file, filename, sizeof file) < sizeof(file)) &&
	    (strlcat(file, ".pub", sizeof file) < sizeof(file)) &&
	    (r = sshkey_try_load_public(pub, file, commentp)) == 0) {
		if (keyp != NULL)
			*keyp = pub;
		return 0;
	}
	sshkey_free(pub);

	return r;
}
