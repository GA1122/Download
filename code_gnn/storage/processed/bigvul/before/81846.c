static int normal_ecc_mulmod(mp_int* k, ecc_point *G, ecc_point *R,
                  mp_int* a, mp_int* modulus, int map,
                  void* heap)
#else
int wc_ecc_mulmod_ex(mp_int* k, ecc_point *G, ecc_point *R,
                  mp_int* a, mp_int* modulus, int map,
                  void* heap)
#endif
{
#ifndef WOLFSSL_SP_MATH
#ifndef ECC_TIMING_RESISTANT
    
   #define WINSIZE  4
   #define M_POINTS 8
   int           first = 1, bitbuf = 0, bitcpy = 0, j;
#else
   #define M_POINTS 3
#endif

   ecc_point     *tG, *M[M_POINTS];
   int           i, err;
   mp_int        mu;
   mp_digit      mp;
   mp_digit      buf;
   int           bitcnt = 0, mode = 0, digidx = 0;

   if (k == NULL || G == NULL || R == NULL || modulus == NULL) {
       return ECC_BAD_ARG_E;
   }

    
   tG = NULL;
   XMEMSET(M, 0, sizeof(M));

    
   if ((err = mp_montgomery_setup(modulus, &mp)) != MP_OKAY) {
       return err;
   }

   if ((err = mp_init(&mu)) != MP_OKAY) {
       return err;
   }
   if ((err = mp_montgomery_calc_normalization(&mu, modulus)) != MP_OKAY) {
       mp_clear(&mu);
       return err;
   }

   
  for (i = 0; i < M_POINTS; i++) {
      M[i] = wc_ecc_new_point_h(heap);
      if (M[i] == NULL) {
         mp_clear(&mu);
         err = MEMORY_E; goto exit;
      }
  }

    
   tG = wc_ecc_new_point_h(heap);
   if (tG == NULL)
       err = MEMORY_E;

    
   if (err == MP_OKAY) {
       if (mp_cmp_d(&mu, 1) == MP_EQ) {
           err = mp_copy(G->x, tG->x);
           if (err == MP_OKAY)
               err = mp_copy(G->y, tG->y);
           if (err == MP_OKAY)
               err = mp_copy(G->z, tG->z);
       } else {
           err = mp_mulmod(G->x, &mu, modulus, tG->x);
           if (err == MP_OKAY)
               err = mp_mulmod(G->y, &mu, modulus, tG->y);
           if (err == MP_OKAY)
               err = mp_mulmod(G->z, &mu, modulus, tG->z);
       }
   }

    
   mp_clear(&mu);

#ifndef ECC_TIMING_RESISTANT

    
    
   if (err == MP_OKAY)
       err = ecc_projective_dbl_point(tG, M[0], a, modulus, mp);
   if (err == MP_OKAY)
       err = ecc_projective_dbl_point(M[0], M[0], a, modulus, mp);
   if (err == MP_OKAY)
       err = ecc_projective_dbl_point(M[0], M[0], a, modulus, mp);

    
   if (err == MP_OKAY)
       for (j = 9; j < 16; j++) {
           err = ecc_projective_add_point(M[j-9], tG, M[j-M_POINTS], a,
                                                                modulus, mp);
           if (err != MP_OKAY) break;
       }

    
   if (err == MP_OKAY) {
       mode   = 0;
       bitcnt = 1;
       buf    = 0;
       digidx = get_digit_count(k) - 1;
       bitcpy = bitbuf = 0;
       first  = 1;

        
       for (;;) {
            
           if (--bitcnt == 0) {
               if (digidx == -1) {
                   break;
               }
               buf    = get_digit(k, digidx);
               bitcnt = (int) DIGIT_BIT;
               --digidx;
           }

            
           i = (int)(buf >> (DIGIT_BIT - 1)) & 1;
           buf <<= 1;

            
           if (mode == 0 && i == 0)
               continue;

            
           if (mode == 1 && i == 0) {
               err = ecc_projective_dbl_point(R, R, a, modulus, mp);
               if (err != MP_OKAY) break;
               continue;
           }

            
           bitbuf |= (i << (WINSIZE - ++bitcpy));
           mode = 2;

           if (bitcpy == WINSIZE) {
                
               if (first == 1) {
                    
                   err = mp_copy(M[bitbuf-M_POINTS]->x, R->x);
                   if (err != MP_OKAY) break;

                   err = mp_copy(M[bitbuf-M_POINTS]->y, R->y);
                   if (err != MP_OKAY) break;

                   err = mp_copy(M[bitbuf-M_POINTS]->z, R->z);
                   first = 0;
               } else {
                    
                    
                    
                   for (j = 0; j < WINSIZE; j++) {
                       err = ecc_projective_dbl_point(R, R, a, modulus, mp);
                       if (err != MP_OKAY) break;
                   }
                   if (err != MP_OKAY) break;   

                    
                   err = ecc_projective_add_point(R, M[bitbuf-M_POINTS], R, a,
                                                               modulus, mp);
               }
               if (err != MP_OKAY) break;
                
               bitcpy = bitbuf = 0;
               mode = 1;
           }
       }
   }

    
   if (err == MP_OKAY) {
       if (mode == 2 && bitcpy > 0) {
            
           for (j = 0; j < bitcpy; j++) {
                
               if (first == 0) {
                   err = ecc_projective_dbl_point(R, R, a, modulus, mp);
                   if (err != MP_OKAY) break;
               }

               bitbuf <<= 1;
               if ((bitbuf & (1 << WINSIZE)) != 0) {
                   if (first == 1) {
                        
                       err = mp_copy(tG->x, R->x);
                       if (err != MP_OKAY) break;

                       err = mp_copy(tG->y, R->y);
                       if (err != MP_OKAY) break;

                       err = mp_copy(tG->z, R->z);
                       if (err != MP_OKAY) break;
                       first = 0;
                   } else {
                        
                       err = ecc_projective_add_point(R, tG, R, a, modulus,
                                                                       mp);
                       if (err != MP_OKAY) break;
                   }
               }
           }
       }
   }

   #undef WINSIZE

#else  

    
    
   if (err == MP_OKAY)
       err = mp_copy(tG->x, M[0]->x);
   if (err == MP_OKAY)
       err = mp_copy(tG->y, M[0]->y);
   if (err == MP_OKAY)
       err = mp_copy(tG->z, M[0]->z);

    
   if (err == MP_OKAY)
       err = ecc_projective_dbl_point(tG, M[1], a, modulus, mp);

    
   mode   = 0;
   bitcnt = 1;
   buf    = 0;
   digidx = get_digit_count(k) - 1;

    
   if (err == MP_OKAY) {
       for (;;) {
            
           if (--bitcnt == 0) {
               if (digidx == -1) {
                   break;
               }
               buf = get_digit(k, digidx);
               bitcnt = (int)DIGIT_BIT;
               --digidx;
           }

            
           i = (buf >> (DIGIT_BIT - 1)) & 1;
           buf <<= 1;

           if (mode == 0 && i == 0) {
                
               if (err == MP_OKAY)
                   err = ecc_projective_add_point(M[0], M[1], M[2], a, modulus,
                                                  mp);
               if (err == MP_OKAY)
                   err = ecc_projective_dbl_point(M[1], M[2], a, modulus, mp);
               if (err == MP_OKAY)
                   continue;
           }

           if (mode == 0 && i == 1) {
               mode = 1;
                
               if (err == MP_OKAY)
                   err = ecc_projective_add_point(M[0], M[1], M[2], a, modulus,
                                                  mp);
               if (err == MP_OKAY)
                   err = ecc_projective_dbl_point(M[1], M[2], a, modulus, mp);
               if (err == MP_OKAY)
                   continue;
           }

           if (err == MP_OKAY)
               err = ecc_projective_add_point(M[0], M[1], M[i^1], a, modulus,
                                                                       mp);
#ifdef WC_NO_CACHE_RESISTANT
           if (err == MP_OKAY)
               err = ecc_projective_dbl_point(M[i], M[i], a, modulus, mp);
#else
             
           if (err == MP_OKAY)
               err = mp_copy((mp_int*)
                             ( ((wolfssl_word)M[0]->x & wc_off_on_addr[i^1]) +
                               ((wolfssl_word)M[1]->x & wc_off_on_addr[i])),
                             M[2]->x);
           if (err == MP_OKAY)
               err = mp_copy((mp_int*)
                             ( ((wolfssl_word)M[0]->y & wc_off_on_addr[i^1]) +
                               ((wolfssl_word)M[1]->y & wc_off_on_addr[i])),
                             M[2]->y);
           if (err == MP_OKAY)
               err = mp_copy((mp_int*)
                             ( ((wolfssl_word)M[0]->z & wc_off_on_addr[i^1]) +
                               ((wolfssl_word)M[1]->z & wc_off_on_addr[i])),
                             M[2]->z);
           if (err == MP_OKAY)
               err = ecc_projective_dbl_point(M[2], M[2], a, modulus, mp);
            
           if (err == MP_OKAY)
               err = mp_copy(M[2]->x,
                             (mp_int*)
                             ( ((wolfssl_word)M[0]->x & wc_off_on_addr[i^1]) +
                               ((wolfssl_word)M[1]->x & wc_off_on_addr[i])) );
           if (err == MP_OKAY)
               err = mp_copy(M[2]->y,
                             (mp_int*)
                             ( ((wolfssl_word)M[0]->y & wc_off_on_addr[i^1]) +
                               ((wolfssl_word)M[1]->y & wc_off_on_addr[i])) );
           if (err == MP_OKAY)
               err = mp_copy(M[2]->z,
                             (mp_int*)
                             ( ((wolfssl_word)M[0]->z & wc_off_on_addr[i^1]) +
                               ((wolfssl_word)M[1]->z & wc_off_on_addr[i])) );
           if (err != MP_OKAY)
               break;
#endif  
       }  
   }

    
   if (err == MP_OKAY)
       err = mp_copy(M[0]->x, R->x);
   if (err == MP_OKAY)
       err = mp_copy(M[0]->y, R->y);
   if (err == MP_OKAY)
       err = mp_copy(M[0]->z, R->z);

#endif  

    
   if (err == MP_OKAY && map)
       err = ecc_map(R, modulus, mp);

exit:

    
   wc_ecc_del_point_h(tG, heap);
   for (i = 0; i < M_POINTS; i++) {
       wc_ecc_del_point_h(M[i], heap);
   }

   return err;
#else
   if (k == NULL || G == NULL || R == NULL || modulus == NULL) {
       return ECC_BAD_ARG_E;
   }

   (void)a;

   return sp_ecc_mulmod_256(k, G, R, map, heap);
#endif
}
