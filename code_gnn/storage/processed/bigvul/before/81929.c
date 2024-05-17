static int wc_ecc_sign_hash_hw(const byte* in, word32 inlen,
    mp_int* r, mp_int* s, byte* out, word32 *outlen, WC_RNG* rng,
    ecc_key* key)
{
    int err;

#ifdef PLUTON_CRYPTO_ECC
    if (key->devId != INVALID_DEVID)  
#endif
    {
        int keysize = key->dp->size;

         
        if (keysize > ECC_MAX_CRYPTO_HW_SIZE || inlen != keysize ||
                                                *outlen < keysize*2) {
            return ECC_BAD_ARG_E;
        }

    #if defined(WOLFSSL_ATECC508A)
         
        err = atcatls_sign(key->slot, in, out);
        if (err != ATCA_SUCCESS) {
           return BAD_COND_E;
        }
    #elif defined(PLUTON_CRYPTO_ECC)
        {
             
            word32 raw_sig_size = *outlen;
            err = Crypto_EccSign(in, inlen, out, &raw_sig_size);
            if (err != CRYPTO_RES_SUCCESS || raw_sig_size != keysize*2){
               return BAD_COND_E;
            }
        }
    #endif

         
        err = mp_read_unsigned_bin(r, &out[0], keysize);
        if (err != MP_OKAY) {
            return err;
        }
        err = mp_read_unsigned_bin(s, &out[keysize], keysize);
        if (err != MP_OKAY) {
            return err;
        }

         
        if (mp_iszero(r) || mp_iszero(s)) {
            return MP_ZERO_E;
        }
    }
#ifdef PLUTON_CRYPTO_ECC
    else {
        err = wc_ecc_sign_hash_ex(in, inlen, rng, key, r, s);
    }
#endif

    return err;
}