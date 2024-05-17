sign_hash(const struct private_key_stuff *pks
	  , const u_char *hash_val, size_t hash_len
	  , u_char *sig_val, size_t sig_len)
{
    chunk_t ch;
    mpz_t t1;
    size_t padlen;
    u_char *p = sig_val;
    const struct RSA_private_key *k = &pks->u.RSA_private_key;

    DBG(DBG_CONTROL | DBG_CRYPT,
	DBG_log("signing hash with RSA Key *%s", pks->pub->u.rsa.keyid)
        );

     
    *p++ = 0x00;
    *p++ = 0x01;	 
    padlen = sig_len - 3 - hash_len;
    memset(p, 0xFF, padlen);
    p += padlen;
    *p++ = 0x00;
    memcpy(p, hash_val, hash_len);
    passert(p + hash_len - sig_val == (ptrdiff_t)sig_len);

     
    n_to_mpz(t1, sig_val, sig_len);	 

     
    oswcrypto.rsa_mod_exp_crt(t1, t1, &k->p, &k->dP, &k->q, &k->dQ, &k->qInv);
     
    ch = mpz_to_n(t1, sig_len);
    memcpy(sig_val, ch.ptr, sig_len);
    pfree(ch.ptr);

    mpz_clear(t1);
}
