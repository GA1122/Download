static int wc_ecc_export_x963_compressed(ecc_key* key, byte* out, word32* outLen)
{
   word32 numlen;
   int    ret = MP_OKAY;

   if (key == NULL || out == NULL || outLen == NULL)
       return BAD_FUNC_ARG;

   if (wc_ecc_is_valid_idx(key->idx) == 0) {
      return ECC_BAD_ARG_E;
   }
   numlen = key->dp->size;

   if (*outLen < (1 + numlen)) {
      *outLen = 1 + numlen;
      return BUFFER_E;
   }

#ifdef WOLFSSL_ATECC508A
    
   ret = BAD_COND_E;

#else

    
   out[0] = mp_isodd(key->pubkey.y) == MP_YES ? ECC_POINT_COMP_ODD : ECC_POINT_COMP_EVEN;

    
   XMEMSET(out+1, 0, numlen);
   ret = mp_to_unsigned_bin(key->pubkey.x,
                       out+1 + (numlen - mp_unsigned_bin_size(key->pubkey.x)));
   *outLen = 1 + numlen;

#endif  

   return ret;
}