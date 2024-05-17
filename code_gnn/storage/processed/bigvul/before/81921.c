int wc_ecc_shared_secret(ecc_key* private_key, ecc_key* public_key, byte* out,
                      word32* outlen)
{
   int err;

   if (private_key == NULL || public_key == NULL || out == NULL ||
                                                            outlen == NULL) {
       return BAD_FUNC_ARG;
   }

#ifdef WOLF_CRYPTO_DEV
    if (private_key->devId != INVALID_DEVID) {
        err = wc_CryptoDev_Ecdh(private_key, public_key, out, outlen);
        if (err != NOT_COMPILED_IN)
            return err;
    }
#endif

    
   if (private_key->type != ECC_PRIVATEKEY &&
           private_key->type != ECC_PRIVATEKEY_ONLY) {
      return ECC_BAD_ARG_E;
   }

    
   if (wc_ecc_is_valid_idx(private_key->idx) == 0 ||
       wc_ecc_is_valid_idx(public_key->idx)  == 0) {
      return ECC_BAD_ARG_E;
   }

    
   if (private_key->dp->id != public_key->dp->id) {
      return ECC_BAD_ARG_E;
   }

#ifdef WOLFSSL_ATECC508A
   err = atcatls_ecdh(private_key->slot, public_key->pubkey_raw, out);
   if (err != ATCA_SUCCESS) {
      err = BAD_COND_E;
   }
   *outlen = private_key->dp->size;
#else
   err = wc_ecc_shared_secret_ex(private_key, &public_key->pubkey, out, outlen);
#endif  

   return err;
}