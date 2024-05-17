int wc_ecc_verify_hash_ex(mp_int *r, mp_int *s, const byte* hash,
                    word32 hashlen, int* res, ecc_key* key)
{
   int           err;
#ifdef WOLFSSL_ATECC508A
   byte sigRS[ATECC_KEY_SIZE*2];
#elif !defined(WOLFSSL_SP_MATH)
   int          did_init = 0;
   ecc_point    *mG = NULL, *mQ = NULL;
   mp_int        v;
   mp_int        w;
   mp_int        u1;
   mp_int        u2;
   mp_int*       e;
#if !defined(WOLFSSL_ASYNC_CRYPT) || !defined(HAVE_CAVIUM_V)
   mp_int        e_lcl;
#endif
   DECLARE_CURVE_SPECS(ECC_CURVE_FIELD_COUNT)
#endif

   if (r == NULL || s == NULL || hash == NULL || res == NULL || key == NULL)
       return ECC_BAD_ARG_E;

    
   *res = 0;

    
   if (wc_ecc_is_valid_idx(key->idx) != 1) {
      return ECC_BAD_ARG_E;
   }

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC) && \
       defined(WOLFSSL_ASYNC_CRYPT_TEST)
    if (key->asyncDev.marker == WOLFSSL_ASYNC_MARKER_ECC) {
        if (wc_AsyncTestInit(&key->asyncDev, ASYNC_TEST_ECC_VERIFY)) {
            WC_ASYNC_TEST* testDev = &key->asyncDev.test;
            testDev->eccVerify.r = r;
            testDev->eccVerify.s = s;
            testDev->eccVerify.hash = hash;
            testDev->eccVerify.hashlen = hashlen;
            testDev->eccVerify.stat = res;
            testDev->eccVerify.key = key;
            return WC_PENDING_E;
        }
    }
#endif

#ifdef WOLFSSL_ATECC508A
     
    err = mp_to_unsigned_bin(r, &sigRS[0]);
    if (err != MP_OKAY) {
        return err;
    }
    err = mp_to_unsigned_bin(s, &sigRS[ATECC_KEY_SIZE]);
    if (err != MP_OKAY) {
        return err;
    }

    err = atcatls_verify(hash, sigRS, key->pubkey_raw, (bool*)res);
    if (err != ATCA_SUCCESS) {
       return BAD_COND_E;
   }

#else

   
  if (key->type == ECC_PRIVATEKEY_ONLY) {
      WOLFSSL_MSG("Verify called with private key, generating public part");
      err = wc_ecc_make_pub_ex(key, NULL, NULL);
      if (err != MP_OKAY) {
           WOLFSSL_MSG("Unable to extract public key");
           return err;
      }
  }

#ifdef WOLFSSL_SP_MATH
  if (key->idx != ECC_CUSTOM_IDX && ecc_sets[key->idx].id == ECC_SECP256R1) {
      return sp_ecc_verify_256(hash, hashlen, key->pubkey.x, key->pubkey.y,
                                           key->pubkey.z, r, s, res, key->heap);
  }
  else
      return WC_KEY_SIZE_E;
#else
#ifdef WOLFSSL_HAVE_SP_ECC
#ifndef WOLFSSL_SP_NO_256
    #if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC) && \
           defined(WOLFSSL_ASYNC_CRYPT_TEST)
    if (key->asyncDev.marker != WOLFSSL_ASYNC_MARKER_ECC)
    #endif
    {
        if (key->idx != ECC_CUSTOM_IDX && ecc_sets[key->idx].id == ECC_SECP256R1)
            return sp_ecc_verify_256(hash, hashlen, key->pubkey.x, key->pubkey.y,
                                     key->pubkey.z,r, s, res, key->heap);
    }
#endif
#endif

#if defined(WOLFSSL_ASYNC_CRYPT) && defined(HAVE_CAVIUM_V)
   err = wc_ecc_alloc_mpint(key, &key->e);
   if (err != 0)
      return err;
   e = key->e;
#else
   e = &e_lcl;
#endif

   err = mp_init(e);
   if (err != MP_OKAY)
      return MEMORY_E;

    
   err = wc_ecc_curve_load(key->dp, &curve, ECC_CURVE_FIELD_ALL);

    
   if (err == MP_OKAY) {
       if (mp_iszero(r) == MP_YES || mp_iszero(s) == MP_YES ||
           mp_cmp(r, curve->order) != MP_LT ||
           mp_cmp(s, curve->order) != MP_LT) {
           err = MP_ZERO_E;
       }
   }

    
   if (err == MP_OKAY) {
        
       unsigned int orderBits = mp_count_bits(curve->order);

        
       if ( (WOLFSSL_BIT_SIZE * hashlen) > orderBits)
           hashlen = (orderBits + WOLFSSL_BIT_SIZE - 1) / WOLFSSL_BIT_SIZE;
       err = mp_read_unsigned_bin(e, hash, hashlen);

        
       if (err == MP_OKAY && (WOLFSSL_BIT_SIZE * hashlen) > orderBits)
           mp_rshb(e, WOLFSSL_BIT_SIZE - (orderBits & 0x7));
   }

    
#if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC)
   if (key->asyncDev.marker == WOLFSSL_ASYNC_MARKER_ECC) {
   #if defined(HAVE_CAVIUM_V) || defined(HAVE_INTEL_QA)
   #ifdef HAVE_CAVIUM_V
      if (NitroxEccIsCurveSupported(key))
   #endif
      {
          word32 keySz = key->dp->size;

          err = wc_mp_to_bigint_sz(e, &e->raw, keySz);
          if (err == MP_OKAY)
              err = wc_mp_to_bigint_sz(key->pubkey.x, &key->pubkey.x->raw, keySz);
          if (err == MP_OKAY)
              err = wc_mp_to_bigint_sz(key->pubkey.y, &key->pubkey.y->raw, keySz);
          if (err == MP_OKAY)
          #ifdef HAVE_CAVIUM_V
              err = NitroxEcdsaVerify(key, &e->raw, &key->pubkey.x->raw,
                    &key->pubkey.y->raw, &r->raw, &s->raw,
                    &curve->prime->raw, &curve->order->raw, res);
          #else
              err = IntelQaEcdsaVerify(&key->asyncDev, &e->raw, &key->pubkey.x->raw,
                    &key->pubkey.y->raw, &r->raw, &s->raw, &curve->Af->raw,
                    &curve->Bf->raw, &curve->prime->raw, &curve->order->raw,
                    &curve->Gx->raw, &curve->Gy->raw, res);
          #endif

      #ifndef HAVE_CAVIUM_V
          mp_clear(e);
      #endif
          wc_ecc_curve_free(curve);

          return err;
      }
   #endif  
   }
#endif  

    
   if (err == MP_OKAY) {
       if ((err = mp_init_multi(&v, &w, &u1, &u2, NULL, NULL)) != MP_OKAY) {
          err = MEMORY_E;
       }
       did_init = 1;
   }

    
   if (err == MP_OKAY) {
       mG = wc_ecc_new_point_h(key->heap);
       mQ = wc_ecc_new_point_h(key->heap);
       if (mQ  == NULL || mG == NULL)
          err = MEMORY_E;
   }

    
   if (err == MP_OKAY)
       err = mp_invmod(s, curve->order, &w);

    
   if (err == MP_OKAY)
       err = mp_mulmod(e, &w, curve->order, &u1);

    
   if (err == MP_OKAY)
       err = mp_mulmod(r, &w, curve->order, &u2);

    
   if (err == MP_OKAY)
       err = mp_copy(curve->Gx, mG->x);
   if (err == MP_OKAY)
       err = mp_copy(curve->Gy, mG->y);
   if (err == MP_OKAY)
       err = mp_set(mG->z, 1);

   if (err == MP_OKAY)
       err = mp_copy(key->pubkey.x, mQ->x);
   if (err == MP_OKAY)
       err = mp_copy(key->pubkey.y, mQ->y);
   if (err == MP_OKAY)
       err = mp_copy(key->pubkey.z, mQ->z);

#ifdef FREESCALE_LTC_ECC
    
   if (err == MP_OKAY)
       err = wc_ecc_mulmod_ex(&u1, mG, mG, curve->Af, curve->prime, 0, key->heap);
   if (err == MP_OKAY)
       err = wc_ecc_mulmod_ex(&u2, mQ, mQ, curve->Af, curve->prime, 0, key->heap);
   if (err == MP_OKAY)
       err = wc_ecc_point_add(mG, mQ, mG, curve->prime);
#else
#ifndef ECC_SHAMIR
    {
        mp_digit      mp = 0;

         
        if (err == MP_OKAY) {
            err = wc_ecc_mulmod_ex(&u1, mG, mG, curve->Af, curve->prime, 0,
                                                                     key->heap);
        }
        if (err == MP_OKAY) {
            err = wc_ecc_mulmod_ex(&u2, mQ, mQ, curve->Af, curve->prime, 0,
                                                                     key->heap);
        }

         
        if (err == MP_OKAY)
            err = mp_montgomery_setup(curve->prime, &mp);

         
        if (err == MP_OKAY)
            err = ecc_projective_add_point(mQ, mG, mG, curve->Af,
                                                             curve->prime, mp);

         
        if (err == MP_OKAY)
            err = ecc_map(mG, curve->prime, mp);
    }
#else
        
        if (err == MP_OKAY) {
            err = ecc_mul2add(mG, &u1, mQ, &u2, mG, curve->Af, curve->prime,
                                                                    key->heap);
        }
#endif  
#endif  
    
   if (err == MP_OKAY)
       err = mp_mod(mG->x, curve->order, &v);

    
   if (err == MP_OKAY) {
       if (mp_cmp(&v, r) == MP_EQ)
           *res = 1;
   }

    
   wc_ecc_del_point_h(mG, key->heap);
   wc_ecc_del_point_h(mQ, key->heap);

   mp_clear(e);
   if (did_init) {
       mp_clear(&v);
       mp_clear(&w);
       mp_clear(&u1);
       mp_clear(&u2);
   }

   wc_ecc_curve_free(curve);

#endif  
#endif  

   return err;
}
