  static int MOD_EXP_CTIME_COPY_TO_PREBUF(const BIGNUM *b, int top,
                                          unsigned char *buf, int idx,
                                        int width)
//                                         int window)
  {
    size_t i, j;
//     int i, j;
//     int width = 1 << window;
//     BN_ULONG *table = (BN_ULONG *)buf;
  
      if (top > b->top)
          top = b->top;            
    for (i = 0, j = idx; i < top * sizeof b->d[0]; i++, j += width) {
        buf[j] = ((unsigned char *)b->d)[i];
//     for (i = 0, j = idx; i < top; i++, j += width) {
//         table[j] = b->d[i];
      }
  
      return 1;
                                           unsigned char *buf, int idx,
  
  static int MOD_EXP_CTIME_COPY_FROM_PREBUF(BIGNUM *b, int top,
                                            unsigned char *buf, int idx,
                                          int width)
//                                           int window)
  {
    size_t i, j;
//     int i, j;
//     int width = 1 << window;
//     volatile BN_ULONG *table = (volatile BN_ULONG *)buf;
  
      if (bn_wexpand(b, top) == NULL)
          return 0;
  
    for (i = 0, j = idx; i < top * sizeof b->d[0]; i++, j += width) {
        ((unsigned char *)b->d)[i] = buf[j];
//     if (window <= 3) {
//         for (i = 0; i < top; i++, table += width) {
//             BN_ULONG acc = 0;
// 
//             for (j = 0; j < width; j++) {
//                 acc |= table[j] &
//                        ((BN_ULONG)0 - (constant_time_eq_int(j,idx)&1));
//             }
// 
//             b->d[i] = acc;
//         }
//     } else {
//         int xstride = 1 << (window - 2);
//         BN_ULONG y0, y1, y2, y3;
// 
//         i = idx >> (window - 2);         
//         idx &= xstride - 1;              
// 
//         y0 = (BN_ULONG)0 - (constant_time_eq_int(i,0)&1);
//         y1 = (BN_ULONG)0 - (constant_time_eq_int(i,1)&1);
//         y2 = (BN_ULONG)0 - (constant_time_eq_int(i,2)&1);
//         y3 = (BN_ULONG)0 - (constant_time_eq_int(i,3)&1);
// 
//         for (i = 0; i < top; i++, table += width) {
//             BN_ULONG acc = 0;
// 
//             for (j = 0; j < xstride; j++) {
//                 acc |= ( (table[j + 0 * xstride] & y0) |
//                          (table[j + 1 * xstride] & y1) |
//                          (table[j + 2 * xstride] & y2) |
//                          (table[j + 3 * xstride] & y3) )
//                        & ((BN_ULONG)0 - (constant_time_eq_int(j,idx)&1));
//             }
// 
//             b->d[i] = acc;
//         }
      }
  
      b->top = top;
     if (!BN_is_odd(m)) {
         BNerr(BN_F_BN_MOD_EXP_MONT_CONSTTIME, BN_R_CALLED_WITH_EVEN_MODULUS);
         return (0);
     }
 
     top = m->top;
 
     bits = BN_num_bits(p);
     if (bits == 0) {
          
         if (BN_is_one(m)) {
             ret = 1;
             BN_zero(rr);
         } else {
             ret = BN_one(rr);
         }
         return ret;
     }
 
     BN_CTX_start(ctx);
 
      
     if (in_mont != NULL)
         mont = in_mont;
     else {
         if ((mont = BN_MONT_CTX_new()) == NULL)
             goto err;
         if (!BN_MONT_CTX_set(mont, m, ctx))
             goto err;
     }
 
 #ifdef RSAZ_ENABLED
      
     if ((16 == a->top) && (16 == p->top) && (BN_num_bits(m) == 1024)
         && rsaz_avx2_eligible()) {
         if (NULL == bn_wexpand(rr, 16))
             goto err;
         RSAZ_1024_mod_exp_avx2(rr->d, a->d, p->d, m->d, mont->RR.d,
                                mont->n0[0]);
         rr->top = 16;
         rr->neg = 0;
         bn_correct_top(rr);
         ret = 1;
         goto err;
     } else if ((8 == a->top) && (8 == p->top) && (BN_num_bits(m) == 512)) {
         if (NULL == bn_wexpand(rr, 8))
             goto err;
         RSAZ_512_mod_exp(rr->d, a->d, p->d, m->d, mont->n0[0], mont->RR.d);
         rr->top = 8;
         rr->neg = 0;
         bn_correct_top(rr);
         ret = 1;
         goto err;
     }
 #endif
 
      
     window = BN_window_bits_for_ctime_exponent_size(bits);
 #if defined(SPARC_T4_MONT)
     if (window >= 5 && (top & 15) == 0 && top <= 64 &&
         (OPENSSL_sparcv9cap_P[1] & (CFR_MONTMUL | CFR_MONTSQR)) ==
         (CFR_MONTMUL | CFR_MONTSQR) && (t4 = OPENSSL_sparcv9cap_P[0]))
         window = 5;
     else
 #endif
 #if defined(OPENSSL_BN_ASM_MONT5)
     if (window >= 5) {
         window = 5;              
         if ((top & 7) == 0)
             powerbufLen += 2 * top * sizeof(m->d[0]);
     }
 #endif
     (void)0;
 
      
     numPowers = 1 << window;
     powerbufLen += sizeof(m->d[0]) * (top * numPowers +
                                       ((2 * top) >
                                        numPowers ? (2 * top) : numPowers));
 #ifdef alloca
     if (powerbufLen < 3072)
         powerbufFree =
             alloca(powerbufLen + MOD_EXP_CTIME_MIN_CACHE_LINE_WIDTH);
     else
 #endif
         if ((powerbufFree =
              (unsigned char *)OPENSSL_malloc(powerbufLen +
                                              MOD_EXP_CTIME_MIN_CACHE_LINE_WIDTH))
             == NULL)
         goto err;
 
     powerbuf = MOD_EXP_CTIME_ALIGN(powerbufFree);
     memset(powerbuf, 0, powerbufLen);
 
 #ifdef alloca
     if (powerbufLen < 3072)
         powerbufFree = NULL;
 #endif
 
      
     tmp.d = (BN_ULONG *)(powerbuf + sizeof(m->d[0]) * top * numPowers);
     am.d = tmp.d + top;
     tmp.top = am.top = 0;
     tmp.dmax = am.dmax = top;
     tmp.neg = am.neg = 0;
     tmp.flags = am.flags = BN_FLG_STATIC_DATA;
 
      
 #if 1                            
     if (m->d[top - 1] & (((BN_ULONG)1) << (BN_BITS2 - 1))) {
          
         tmp.d[0] = (0 - m->d[0]) & BN_MASK2;
         for (i = 1; i < top; i++)
             tmp.d[i] = (~m->d[i]) & BN_MASK2;
         tmp.top = top;
     } else
 #endif
     if (!BN_to_montgomery(&tmp, BN_value_one(), mont, ctx))
         goto err;
 
      
     if (a->neg || BN_ucmp(a, m) >= 0) {
         if (!BN_mod(&am, a, m, ctx))
             goto err;
         if (!BN_to_montgomery(&am, &am, mont, ctx))
             goto err;
     } else if (!BN_to_montgomery(&am, a, mont, ctx))
         goto err;
 
 #if defined(SPARC_T4_MONT)
     if (t4) {
         typedef int (*bn_pwr5_mont_f) (BN_ULONG *tp, const BN_ULONG *np,
                                        const BN_ULONG *n0, const void *table,
                                        int power, int bits);
         int bn_pwr5_mont_t4_8(BN_ULONG *tp, const BN_ULONG *np,
                               const BN_ULONG *n0, const void *table,
                               int power, int bits);
         int bn_pwr5_mont_t4_16(BN_ULONG *tp, const BN_ULONG *np,
                                const BN_ULONG *n0, const void *table,
                                int power, int bits);
         int bn_pwr5_mont_t4_24(BN_ULONG *tp, const BN_ULONG *np,
                                const BN_ULONG *n0, const void *table,
                                int power, int bits);
         int bn_pwr5_mont_t4_32(BN_ULONG *tp, const BN_ULONG *np,
                                const BN_ULONG *n0, const void *table,
                                int power, int bits);
         static const bn_pwr5_mont_f pwr5_funcs[4] = {
             bn_pwr5_mont_t4_8, bn_pwr5_mont_t4_16,
             bn_pwr5_mont_t4_24, bn_pwr5_mont_t4_32
         };
         bn_pwr5_mont_f pwr5_worker = pwr5_funcs[top / 16 - 1];
 
         typedef int (*bn_mul_mont_f) (BN_ULONG *rp, const BN_ULONG *ap,
                                       const void *bp, const BN_ULONG *np,
                                       const BN_ULONG *n0);
         int bn_mul_mont_t4_8(BN_ULONG *rp, const BN_ULONG *ap, const void *bp,
                              const BN_ULONG *np, const BN_ULONG *n0);
         int bn_mul_mont_t4_16(BN_ULONG *rp, const BN_ULONG *ap,
                               const void *bp, const BN_ULONG *np,
                               const BN_ULONG *n0);
         int bn_mul_mont_t4_24(BN_ULONG *rp, const BN_ULONG *ap,
                               const void *bp, const BN_ULONG *np,
                               const BN_ULONG *n0);
         int bn_mul_mont_t4_32(BN_ULONG *rp, const BN_ULONG *ap,
                               const void *bp, const BN_ULONG *np,
                               const BN_ULONG *n0);
         static const bn_mul_mont_f mul_funcs[4] = {
             bn_mul_mont_t4_8, bn_mul_mont_t4_16,
             bn_mul_mont_t4_24, bn_mul_mont_t4_32
         };
         bn_mul_mont_f mul_worker = mul_funcs[top / 16 - 1];
 
         void bn_mul_mont_vis3(BN_ULONG *rp, const BN_ULONG *ap,
                               const void *bp, const BN_ULONG *np,
                               const BN_ULONG *n0, int num);
         void bn_mul_mont_t4(BN_ULONG *rp, const BN_ULONG *ap,
                             const void *bp, const BN_ULONG *np,
                             const BN_ULONG *n0, int num);
         void bn_mul_mont_gather5_t4(BN_ULONG *rp, const BN_ULONG *ap,
                                     const void *table, const BN_ULONG *np,
                                     const BN_ULONG *n0, int num, int power);
         void bn_flip_n_scatter5_t4(const BN_ULONG *inp, size_t num,
                                    void *table, size_t power);
         void bn_gather5_t4(BN_ULONG *out, size_t num,
                            void *table, size_t power);
         void bn_flip_t4(BN_ULONG *dst, BN_ULONG *src, size_t num);
 
         BN_ULONG *np = mont->N.d, *n0 = mont->n0;
         int stride = 5 * (6 - (top / 16 - 1));  
 
          
         for (i = am.top; i < top; i++)
             am.d[i] = 0;
         for (i = tmp.top; i < top; i++)
             tmp.d[i] = 0;
 
         bn_flip_n_scatter5_t4(tmp.d, top, powerbuf, 0);
         bn_flip_n_scatter5_t4(am.d, top, powerbuf, 1);
         if (!(*mul_worker) (tmp.d, am.d, am.d, np, n0) &&
             !(*mul_worker) (tmp.d, am.d, am.d, np, n0))
             bn_mul_mont_vis3(tmp.d, am.d, am.d, np, n0, top);
         bn_flip_n_scatter5_t4(tmp.d, top, powerbuf, 2);
 
         for (i = 3; i < 32; i++) {
              
             if (!(*mul_worker) (tmp.d, tmp.d, am.d, np, n0) &&
                 !(*mul_worker) (tmp.d, tmp.d, am.d, np, n0))
                 bn_mul_mont_vis3(tmp.d, tmp.d, am.d, np, n0, top);
             bn_flip_n_scatter5_t4(tmp.d, top, powerbuf, i);
         }
 
          
         np = alloca(top * sizeof(BN_ULONG));
         top /= 2;
         bn_flip_t4(np, mont->N.d, top);
 
         bits--;
         for (wvalue = 0, i = bits % 5; i >= 0; i--, bits--)
             wvalue = (wvalue << 1) + BN_is_bit_set(p, bits);
         bn_gather5_t4(tmp.d, top, powerbuf, wvalue);
 
          
         while (bits >= 0) {
             if (bits < stride)
                 stride = bits + 1;
             bits -= stride;
             wvalue = bn_get_bits(p, bits + 1);
 
             if ((*pwr5_worker) (tmp.d, np, n0, powerbuf, wvalue, stride))
                 continue;
              
             if ((*pwr5_worker) (tmp.d, np, n0, powerbuf, wvalue, stride))
                 continue;
 
             bits += stride - 5;
             wvalue >>= stride - 5;
             wvalue &= 31;
             bn_mul_mont_t4(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_mul_mont_t4(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_mul_mont_t4(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_mul_mont_t4(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_mul_mont_t4(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_mul_mont_gather5_t4(tmp.d, tmp.d, powerbuf, np, n0, top,
                                    wvalue);
         }
 
         bn_flip_t4(tmp.d, tmp.d, top);
         top *= 2;
          
         tmp.top = top;
         bn_correct_top(&tmp);
         OPENSSL_cleanse(np, top * sizeof(BN_ULONG));
     } else
 #endif
 #if defined(OPENSSL_BN_ASM_MONT5)
     if (window == 5 && top > 1) {
          
 
          
         void bn_mul_mont_gather5(BN_ULONG *rp, const BN_ULONG *ap,
                                  const void *table, const BN_ULONG *np,
                                  const BN_ULONG *n0, int num, int power);
         void bn_scatter5(const BN_ULONG *inp, size_t num,
                          void *table, size_t power);
         void bn_gather5(BN_ULONG *out, size_t num, void *table, size_t power);
         void bn_power5(BN_ULONG *rp, const BN_ULONG *ap,
                        const void *table, const BN_ULONG *np,
                        const BN_ULONG *n0, int num, int power);
         int bn_get_bits5(const BN_ULONG *ap, int off);
         int bn_from_montgomery(BN_ULONG *rp, const BN_ULONG *ap,
                                const BN_ULONG *not_used, const BN_ULONG *np,
                                const BN_ULONG *n0, int num);
 
         BN_ULONG *np = mont->N.d, *n0 = mont->n0, *np2;
 
          
         for (i = am.top; i < top; i++)
             am.d[i] = 0;
         for (i = tmp.top; i < top; i++)
             tmp.d[i] = 0;
 
         if (top & 7)
             np2 = np;
         else
             for (np2 = am.d + top, i = 0; i < top; i++)
                 np2[2 * i] = np[i];
 
         bn_scatter5(tmp.d, top, powerbuf, 0);
         bn_scatter5(am.d, am.top, powerbuf, 1);
         bn_mul_mont(tmp.d, am.d, am.d, np, n0, top);
         bn_scatter5(tmp.d, top, powerbuf, 2);
 
 # if 0
         for (i = 3; i < 32; i++) {
              
             bn_mul_mont_gather5(tmp.d, am.d, powerbuf, np2, n0, top, i - 1);
             bn_scatter5(tmp.d, top, powerbuf, i);
         }
 # else
          
         for (i = 4; i < 32; i *= 2) {
             bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_scatter5(tmp.d, top, powerbuf, i);
         }
         for (i = 3; i < 8; i += 2) {
             int j;
             bn_mul_mont_gather5(tmp.d, am.d, powerbuf, np2, n0, top, i - 1);
             bn_scatter5(tmp.d, top, powerbuf, i);
             for (j = 2 * i; j < 32; j *= 2) {
                 bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
                 bn_scatter5(tmp.d, top, powerbuf, j);
             }
         }
         for (; i < 16; i += 2) {
             bn_mul_mont_gather5(tmp.d, am.d, powerbuf, np2, n0, top, i - 1);
             bn_scatter5(tmp.d, top, powerbuf, i);
             bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
             bn_scatter5(tmp.d, top, powerbuf, 2 * i);
         }
         for (; i < 32; i += 2) {
             bn_mul_mont_gather5(tmp.d, am.d, powerbuf, np2, n0, top, i - 1);
             bn_scatter5(tmp.d, top, powerbuf, i);
         }
 # endif
         bits--;
         for (wvalue = 0, i = bits % 5; i >= 0; i--, bits--)
             wvalue = (wvalue << 1) + BN_is_bit_set(p, bits);
         bn_gather5(tmp.d, top, powerbuf, wvalue);
 
          
         if (top & 7)
             while (bits >= 0) {
                 for (wvalue = 0, i = 0; i < 5; i++, bits--)
                     wvalue = (wvalue << 1) + BN_is_bit_set(p, bits);
 
                 bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
                 bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
                 bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
                 bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
                 bn_mul_mont(tmp.d, tmp.d, tmp.d, np, n0, top);
                 bn_mul_mont_gather5(tmp.d, tmp.d, powerbuf, np, n0, top,
                                     wvalue);
         } else {
             while (bits >= 0) {
                 wvalue = bn_get_bits5(p->d, bits - 4);
                 bits -= 5;
                 bn_power5(tmp.d, tmp.d, powerbuf, np2, n0, top, wvalue);
             }
         }
 
         ret = bn_from_montgomery(tmp.d, tmp.d, NULL, np2, n0, top);
         tmp.top = top;
         bn_correct_top(&tmp);
         if (ret) {
             if (!BN_copy(rr, &tmp))
                 ret = 0;
             goto err;            
         }
     } else
 #endif
     {
         if (!MOD_EXP_CTIME_COPY_TO_PREBUF(&tmp, top, powerbuf, 0, numPowers))
             goto err;
         if (!MOD_EXP_CTIME_COPY_TO_PREBUF(&am, top, powerbuf, 1, numPowers))
             goto err;
 
          
         if (window > 1) {
             if (!BN_mod_mul_montgomery(&tmp, &am, &am, mont, ctx))
                 goto err;
             if (!MOD_EXP_CTIME_COPY_TO_PREBUF
                 (&tmp, top, powerbuf, 2, numPowers))
                 goto err;
             for (i = 3; i < numPowers; i++) {
                  
                 if (!BN_mod_mul_montgomery(&tmp, &am, &tmp, mont, ctx))
                     goto err;
                 if (!MOD_EXP_CTIME_COPY_TO_PREBUF
                     (&tmp, top, powerbuf, i, numPowers))
                     goto err;
             }
         }
 
         bits--;
         for (wvalue = 0, i = bits % window; i >= 0; i--, bits--)
             wvalue = (wvalue << 1) + BN_is_bit_set(p, bits);
         if (!MOD_EXP_CTIME_COPY_FROM_PREBUF
             (&tmp, top, powerbuf, wvalue, numPowers))
             goto err;
 
          
                  if (!BN_mod_mul_montgomery(&tmp, &am, &tmp, mont, ctx))
                      goto err;
                if (!MOD_EXP_CTIME_COPY_TO_PREBUF
                    (&tmp, top, powerbuf, i, numPowers))
//                 if (!MOD_EXP_CTIME_COPY_TO_PREBUF(&tmp, top, powerbuf, i,
//                                                   window))
                      goto err;
              }
          }
         for (i = 1; i < top; i++)
          bits--;
          for (wvalue = 0, i = bits % window; i >= 0; i--, bits--)
              wvalue = (wvalue << 1) + BN_is_bit_set(p, bits);
        if (!MOD_EXP_CTIME_COPY_FROM_PREBUF
            (&tmp, top, powerbuf, wvalue, numPowers))
//         if (!MOD_EXP_CTIME_COPY_FROM_PREBUF(&tmp, top, powerbuf, wvalue,
//                                             window))
              goto err;
  
           
            if (!MOD_EXP_CTIME_COPY_FROM_PREBUF
                (&am, top, powerbuf, wvalue, numPowers))
//             if (!MOD_EXP_CTIME_COPY_FROM_PREBUF(&am, top, powerbuf, wvalue,
//                                                 window))
                  goto err;
  
               
 #define BN_MOD_MUL_WORD(r, w, m) \
                 (BN_mul_word(r, (w)) && \
                 (   \
                         (BN_mod(t, r, m, ctx) && (swap_tmp = r, r = t, t = swap_tmp, 1))))
      
      
 #define BN_TO_MONTGOMERY_WORD(r, w, mont) \
                 (BN_set_word(r, (w)) && BN_to_montgomery(r, r, (mont), ctx))
 
     if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0) {
          
         BNerr(BN_F_BN_MOD_EXP_MONT_WORD, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
         return -1;
     }
 
     bn_check_top(p);
     bn_check_top(m);
 
     if (!BN_is_odd(m)) {
         BNerr(BN_F_BN_MOD_EXP_MONT_WORD, BN_R_CALLED_WITH_EVEN_MODULUS);
         return (0);
     }
     if (m->top == 1)
         a %= m->d[0];            
 
     bits = BN_num_bits(p);
     if (bits == 0) {
          
         if (BN_is_one(m)) {
             ret = 1;
             BN_zero(rr);
         } else {
             ret = BN_one(rr);
         }
         return ret;
     }
     if (a == 0) {
         BN_zero(rr);
         ret = 1;
         return ret;
     }
 
     BN_CTX_start(ctx);
     d = BN_CTX_get(ctx);
     r = BN_CTX_get(ctx);
     t = BN_CTX_get(ctx);
     if (d == NULL || r == NULL || t == NULL)
         goto err;
 
     if (in_mont != NULL)
         mont = in_mont;
     else {
         if ((mont = BN_MONT_CTX_new()) == NULL)
             goto err;
         if (!BN_MONT_CTX_set(mont, m, ctx))
             goto err;
     }
 
     r_is_one = 1;                
 
      
 
      
     w = a;                       
     for (b = bits - 2; b >= 0; b--) {
          
         next_w = w * w;
         if ((next_w / w) != w) {  
             if (r_is_one) {
                 if (!BN_TO_MONTGOMERY_WORD(r, w, mont))
                     goto err;
                 r_is_one = 0;
             } else {
                 if (!BN_MOD_MUL_WORD(r, w, m))
                     goto err;
             }
             next_w = 1;
         }
         w = next_w;
         if (!r_is_one) {
             if (!BN_mod_mul_montgomery(r, r, r, mont, ctx))
                 goto err;
         }
 
          
         if (BN_is_bit_set(p, b)) {
             next_w = w * a;
             if ((next_w / a) != w) {  
                 if (r_is_one) {
                     if (!BN_TO_MONTGOMERY_WORD(r, w, mont))
                         goto err;
                     r_is_one = 0;
                 } else {
                     if (!BN_MOD_MUL_WORD(r, w, m))
                         goto err;
                 }
                 next_w = a;
             }
             w = next_w;
         }
     }
 
      
     if (w != 1) {
         if (r_is_one) {
             if (!BN_TO_MONTGOMERY_WORD(r, w, mont))
                 goto err;
             r_is_one = 0;
         } else {
             if (!BN_MOD_MUL_WORD(r, w, m))
                 goto err;
         }
     }
 
     if (r_is_one) {              
         if (!BN_one(rr))
             goto err;
     } else {
         if (!BN_from_montgomery(rr, r, mont, ctx))
             goto err;
     }
     ret = 1;
  err:
     if ((in_mont == NULL) && (mont != NULL))
         BN_MONT_CTX_free(mont);
     BN_CTX_end(ctx);
     bn_check_top(rr);
     return (ret);
 }