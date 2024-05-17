static int accel_fp_mul2add(int idx1, int idx2,
                            mp_int* kA, mp_int* kB,
                            ecc_point *R, mp_int* a,
                            mp_int* modulus, mp_digit mp)
{
#define KB_SIZE 128

#ifdef WOLFSSL_SMALL_STACK
   unsigned char* kb[2] = {NULL, NULL};
#else
   unsigned char kb[2][KB_SIZE];
#endif
   int      x, err;
   unsigned y, z, bitlen, bitpos, lut_gap, first, zA, zB;
   mp_int tka, tkb, order;

   if (mp_init_multi(&tka, &tkb, &order, NULL, NULL, NULL) != MP_OKAY)
       return MP_INIT_E;

    
   if (mp_unsigned_bin_size(kA) > mp_unsigned_bin_size(modulus)) {
       
      y = mp_unsigned_bin_size(modulus);
      for (x = 0; ecc_sets[x].size; x++) {
         if (y <= (unsigned)ecc_sets[x].size) break;
      }

       
      if (y == 66) --x;

      if ((err = mp_read_radix(&order, ecc_sets[x].order,
                                                MP_RADIX_HEX)) != MP_OKAY) {
         goto done;
      }

       
      if (mp_cmp(kA, &order) != MP_LT) {
         if ((err = mp_mod(kA, &order, &tka)) != MP_OKAY) {
            goto done;
         }
      } else {
         if ((err = mp_copy(kA, &tka)) != MP_OKAY) {
            goto done;
         }
      }
   } else {
      if ((err = mp_copy(kA, &tka)) != MP_OKAY) {
         goto done;
      }
   }

    
   if (mp_unsigned_bin_size(kB) > mp_unsigned_bin_size(modulus)) {
       
      y = mp_unsigned_bin_size(modulus);
      for (x = 0; ecc_sets[x].size; x++) {
         if (y <= (unsigned)ecc_sets[x].size) break;
      }

       
      if (y == 66) --x;

      if ((err = mp_read_radix(&order, ecc_sets[x].order,
                                                MP_RADIX_HEX)) != MP_OKAY) {
         goto done;
      }

       
      if (mp_cmp(kB, &order) != MP_LT) {
         if ((err = mp_mod(kB, &order, &tkb)) != MP_OKAY) {
            goto done;
         }
      } else {
         if ((err = mp_copy(kB, &tkb)) != MP_OKAY) {
            goto done;
         }
      }
   } else {
      if ((err = mp_copy(kB, &tkb)) != MP_OKAY) {
         goto done;
      }
   }

    
   bitlen  = mp_unsigned_bin_size(modulus) << 3;
   x       = bitlen % FP_LUT;
   if (x) {
      bitlen += FP_LUT - x;
   }
   lut_gap = bitlen / FP_LUT;

    
   if ((mp_unsigned_bin_size(&tka) > (int)(KB_SIZE - 2)) ||
       (mp_unsigned_bin_size(&tkb) > (int)(KB_SIZE - 2))  ) {
      err = BUFFER_E; goto done;
   }

    
#ifdef WOLFSSL_SMALL_STACK
   kb[0] = (unsigned char*)XMALLOC(KB_SIZE, NULL, DYNAMIC_TYPE_ECC_BUFFER);
   if (kb[0] == NULL) {
      err = MEMORY_E; goto done;
   }
#endif

   XMEMSET(kb[0], 0, KB_SIZE);
   if ((err = mp_to_unsigned_bin(&tka, kb[0])) != MP_OKAY) {
      goto done;
   }

    
   x = 0;
   y = mp_unsigned_bin_size(&tka);
   if (y > 0) {
       y -= 1;
   }
   mp_clear(&tka);
   while ((unsigned)x < y) {
      z = kb[0][x]; kb[0][x] = kb[0][y]; kb[0][y] = (byte)z;
      ++x; --y;
   }

    
#ifdef WOLFSSL_SMALL_STACK
   kb[1] = (unsigned char*)XMALLOC(KB_SIZE, NULL, DYNAMIC_TYPE_ECC_BUFFER);
   if (kb[1] == NULL) {
      err = MEMORY_E; goto done;
   }
#endif

   XMEMSET(kb[1], 0, KB_SIZE);
   if ((err = mp_to_unsigned_bin(&tkb, kb[1])) == MP_OKAY) {
      x = 0;
      y = mp_unsigned_bin_size(&tkb);
      if (y > 0) {
          y -= 1;
      }

      while ((unsigned)x < y) {
         z = kb[1][x]; kb[1][x] = kb[1][y]; kb[1][y] = (byte)z;
         ++x; --y;
      }

       
      first = 1;
      for (x = lut_gap-1; x >= 0; x--) {
           
          bitpos = x;
          for (y = zA = zB = 0; y < FP_LUT; y++) {
             zA |= ((kb[0][bitpos>>3] >> (bitpos&7)) & 1) << y;
             zB |= ((kb[1][bitpos>>3] >> (bitpos&7)) & 1) << y;
             bitpos += lut_gap;     
          }

           
          if (!first) {
             if ((err = ecc_projective_dbl_point(R, R, a, modulus,
                                                              mp)) != MP_OKAY) {
                break;
             }
          }

           
          if (!first) {
             if (zA) {
                if ((err = ecc_projective_add_point(R, fp_cache[idx1].LUT[zA],
                                                  R, a, modulus, mp)) != MP_OKAY) {
                   break;
                }
             }
             if (zB) {
                if ((err = ecc_projective_add_point(R, fp_cache[idx2].LUT[zB],
                                                  R, a, modulus, mp)) != MP_OKAY) {
                   break;
                }
             }
          } else {
             if (zA) {
                 if ((mp_copy(fp_cache[idx1].LUT[zA]->x, R->x) != MP_OKAY) ||
                    (mp_copy(fp_cache[idx1].LUT[zA]->y,  R->y) != MP_OKAY) ||
                    (mp_copy(&fp_cache[idx1].mu,         R->z) != MP_OKAY)) {
                     err = GEN_MEM_ERR;
                     break;
                 }
                    first = 0;
             }
             if (zB && first == 0) {
                if (zB) {
                   if ((err = ecc_projective_add_point(R,
                           fp_cache[idx2].LUT[zB], R, a, modulus, mp)) != MP_OKAY){
                      break;
                   }
                }
             } else if (zB && first == 1) {
                 if ((mp_copy(fp_cache[idx2].LUT[zB]->x, R->x) != MP_OKAY) ||
                    (mp_copy(fp_cache[idx2].LUT[zB]->y, R->y) != MP_OKAY) ||
                    (mp_copy(&fp_cache[idx2].mu,        R->z) != MP_OKAY)) {
                     err = GEN_MEM_ERR;
                     break;
                 }
                    first = 0;
             }
          }
      }
   }

done:
    
   mp_clear(&tkb);
   mp_clear(&tka);
   mp_clear(&order);

#ifdef WOLFSSL_SMALL_STACK
   if (kb[0])
#endif
      ForceZero(kb[0], KB_SIZE);
#ifdef WOLFSSL_SMALL_STACK
   if (kb[1])
#endif
      ForceZero(kb[1], KB_SIZE);

#ifdef WOLFSSL_SMALL_STACK
   XFREE(kb[0], NULL, DYNAMIC_TYPE_ECC_BUFFER);
   XFREE(kb[1], NULL, DYNAMIC_TYPE_ECC_BUFFER);
#endif

#undef KB_SIZE

    if (err != MP_OKAY)
        return err;

   return ecc_map(R, modulus, mp);
}