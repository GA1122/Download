int wc_ecc_export_x963(ecc_key* key, byte* out, word32* outLen)
{
   int    ret = MP_OKAY;
   word32 numlen;
#ifdef WOLFSSL_SMALL_STACK
   byte*  buf;
#else
   byte   buf[ECC_BUFSIZE];
#endif
   word32 pubxlen, pubylen;

    
   if (key != NULL && out == NULL && outLen != NULL) {
      numlen = key->dp->size;
      *outLen = 1 + 2*numlen;
      return LENGTH_ONLY_E;
   }

   if (key == NULL || out == NULL || outLen == NULL)
      return ECC_BAD_ARG_E;

   if (key->type == ECC_PRIVATEKEY_ONLY)
       return ECC_PRIVATEONLY_E;

   if (wc_ecc_is_valid_idx(key->idx) == 0) {
      return ECC_BAD_ARG_E;
   }
   numlen = key->dp->size;

     
   if (*outLen < (1 + 2*numlen)) {
      *outLen = 1 + 2*numlen;
      return BUFFER_E;
   }

    
   pubxlen = mp_unsigned_bin_size(key->pubkey.x);
   pubylen = mp_unsigned_bin_size(key->pubkey.y);
   if ((pubxlen > numlen) || (pubylen > numlen)) {
      WOLFSSL_MSG("Public key x/y invalid!");
      return BUFFER_E;
   }

    
   out[0] = ECC_POINT_UNCOMP;

#ifdef WOLFSSL_SMALL_STACK
   buf = (byte*)XMALLOC(ECC_BUFSIZE, NULL, DYNAMIC_TYPE_ECC_BUFFER);
   if (buf == NULL)
      return MEMORY_E;
#endif

    
   XMEMSET(buf, 0, ECC_BUFSIZE);
   ret = mp_to_unsigned_bin(key->pubkey.x, buf + (numlen - pubxlen));
   if (ret != MP_OKAY)
      goto done;
   XMEMCPY(out+1, buf, numlen);

    
   XMEMSET(buf, 0, ECC_BUFSIZE);
   ret = mp_to_unsigned_bin(key->pubkey.y, buf + (numlen - pubylen));
   if (ret != MP_OKAY)
      goto done;
   XMEMCPY(out+1+numlen, buf, numlen);

   *outLen = 1 + 2*numlen;

done:
#ifdef WOLFSSL_SMALL_STACK
   XFREE(buf, NULL, DYNAMIC_TYPE_ECC_BUFFER);
#endif

   return ret;
}