agent_decode_rsa1(struct sshbuf *m, struct sshkey **kp)
{
	struct sshkey *k = NULL;
	int r = SSH_ERR_INTERNAL_ERROR;

	*kp = NULL;
	if ((k = sshkey_new_private(KEY_RSA1)) == NULL)
		return SSH_ERR_ALLOC_FAIL;

	if ((r = sshbuf_get_u32(m, NULL)) != 0 ||		 
	    (r = sshbuf_get_bignum1(m, k->rsa->n)) != 0 ||
	    (r = sshbuf_get_bignum1(m, k->rsa->e)) != 0 ||
	    (r = sshbuf_get_bignum1(m, k->rsa->d)) != 0 ||
	    (r = sshbuf_get_bignum1(m, k->rsa->iqmp)) != 0 ||
	     
	    (r = sshbuf_get_bignum1(m, k->rsa->q)) != 0 ||	 
	    (r = sshbuf_get_bignum1(m, k->rsa->p)) != 0) 	 
		goto out;

	 
	if ((r = rsa_generate_additional_parameters(k->rsa)) != 0)
		goto out;
	 
	if (RSA_blinding_on(k->rsa, NULL) != 1) {
		r = SSH_ERR_LIBCRYPTO_ERROR;
		goto out;
	}

	r = 0;  
 out:
	if (r == 0)
		*kp = k;
	else
		sshkey_free(k);
	return r;
}