static int normal_ecc_mul2add(ecc_point* A, mp_int* kA,
                             ecc_point* B, mp_int* kB,
                             ecc_point* C, mp_int* a, mp_int* modulus,
                             void* heap)
#else
int ecc_mul2add(ecc_point* A, mp_int* kA,
                    ecc_point* B, mp_int* kB,
                    ecc_point* C, mp_int* a, mp_int* modulus,
                    void* heap)
#endif
{
  ecc_point*     precomp[16];
  unsigned       bitbufA, bitbufB, lenA, lenB, len, nA, nB, nibble;
  unsigned char* tA;
  unsigned char* tB;
  int            err = MP_OKAY, first, x, y;
  mp_digit mp = 0;

   
  if (A == NULL || kA == NULL || B == NULL || kB == NULL || C == NULL ||
                                                         modulus == NULL) {
     return ECC_BAD_ARG_E;
  }

   
  tA = (unsigned char*)XMALLOC(ECC_BUFSIZE, heap, DYNAMIC_TYPE_ECC_BUFFER);
  if (tA == NULL) {
     return GEN_MEM_ERR;
  }
  tB = (unsigned char*)XMALLOC(ECC_BUFSIZE, heap, DYNAMIC_TYPE_ECC_BUFFER);
  if (tB == NULL) {
     XFREE(tA, heap, DYNAMIC_TYPE_ECC_BUFFER);
     return GEN_MEM_ERR;
  }

   
  XMEMSET(tA, 0, ECC_BUFSIZE);
  XMEMSET(tB, 0, ECC_BUFSIZE);
  XMEMSET(precomp, 0, sizeof(precomp));

   
  lenA = mp_unsigned_bin_size(kA);
  lenB = mp_unsigned_bin_size(kB);
  len  = MAX(lenA, lenB);

   
  if ((lenA > ECC_BUFSIZE) || (lenB > ECC_BUFSIZE)) {
    err = BAD_FUNC_ARG;
  }

  if (err == MP_OKAY) {
     
    err = mp_to_unsigned_bin(kA, (len - lenA) + tA);

     
    if (err == MP_OKAY)
        err = mp_to_unsigned_bin(kB, (len - lenB) + tB);

     
    if (err == MP_OKAY) {
        for (x = 0; x < 16; x++) {
            precomp[x] = wc_ecc_new_point_h(heap);
            if (precomp[x] == NULL) {
                err = GEN_MEM_ERR;
                break;
            }
        }
    }
  }

  if (err == MP_OKAY)
     
    err = mp_montgomery_setup(modulus, &mp);

  if (err == MP_OKAY) {
    mp_int mu;
    err = mp_init(&mu);
    if (err == MP_OKAY) {
      err = mp_montgomery_calc_normalization(&mu, modulus);

      if (err == MP_OKAY)
         
        err = mp_mulmod(A->x, &mu, modulus, precomp[1]->x);

      if (err == MP_OKAY)
        err = mp_mulmod(A->y, &mu, modulus, precomp[1]->y);
      if (err == MP_OKAY)
        err = mp_mulmod(A->z, &mu, modulus, precomp[1]->z);

      if (err == MP_OKAY)
        err = mp_mulmod(B->x, &mu, modulus, precomp[1<<2]->x);
      if (err == MP_OKAY)
        err = mp_mulmod(B->y, &mu, modulus, precomp[1<<2]->y);
      if (err == MP_OKAY)
        err = mp_mulmod(B->z, &mu, modulus, precomp[1<<2]->z);

       
      mp_clear(&mu);
    }
  }

  if (err == MP_OKAY)
     
    err = ecc_projective_dbl_point(precomp[1], precomp[2], a, modulus, mp);

  if (err == MP_OKAY)
    err = ecc_projective_add_point(precomp[1], precomp[2], precomp[3],
                                   a, modulus, mp);
  if (err == MP_OKAY)
     
    err = ecc_projective_dbl_point(precomp[1<<2], precomp[2<<2], a, modulus, mp);

  if (err == MP_OKAY)
    err = ecc_projective_add_point(precomp[1<<2], precomp[2<<2], precomp[3<<2],
                                   a, modulus, mp);

  if (err == MP_OKAY) {
     
    for (x = 1; x < 4; x++) {
        for (y = 1; y < 4; y++) {
            if (err == MP_OKAY)
                err = ecc_projective_add_point(precomp[x], precomp[(y<<2)],
                                               precomp[x+(y<<2)], a, modulus, mp);
        }
    }
  }

  if (err == MP_OKAY) {
    nibble  = 3;
    first   = 1;
    bitbufA = tA[0];
    bitbufB = tB[0];

     
    for (x = 0;; ) {
         
        if (++nibble == 4) {
            if (x == (int)len) break;
            bitbufA = tA[x];
            bitbufB = tB[x];
            nibble  = 0;
            x++;
        }

         
        nA = (bitbufA >> 6) & 0x03;
        nB = (bitbufB >> 6) & 0x03;
        bitbufA = (bitbufA << 2) & 0xFF;
        bitbufB = (bitbufB << 2) & 0xFF;

         
        if ((nA == 0) && (nB == 0) && (first == 1)) {
            continue;
        }

         
        if (first == 0) {
             
            if (err == MP_OKAY)
                err = ecc_projective_dbl_point(C, C, a, modulus, mp);
            if (err == MP_OKAY)
                err = ecc_projective_dbl_point(C, C, a, modulus, mp);
            else
                break;
        }

         
        if ((nA != 0) || (nB != 0)) {
            if (first == 1) {
                 
                first = 0;
                if (err == MP_OKAY)
                    err = mp_copy(precomp[nA + (nB<<2)]->x, C->x);

                if (err == MP_OKAY)
                    err = mp_copy(precomp[nA + (nB<<2)]->y, C->y);

                if (err == MP_OKAY)
                    err = mp_copy(precomp[nA + (nB<<2)]->z, C->z);
                else
                    break;
            } else {
                 
                if (err == MP_OKAY)
                    err = ecc_projective_add_point(C, precomp[nA + (nB<<2)], C,
                                                   a, modulus, mp);
                else
                    break;
            }
        }
    }
  }

   
  if (err == MP_OKAY)
    err = ecc_map(C, modulus, mp);

   
  for (x = 0; x < 16; x++) {
     wc_ecc_del_point_h(precomp[x], heap);
  }

  ForceZero(tA, ECC_BUFSIZE);
  ForceZero(tB, ECC_BUFSIZE);
  XFREE(tA, heap, DYNAMIC_TYPE_ECC_BUFFER);
  XFREE(tB, heap, DYNAMIC_TYPE_ECC_BUFFER);

  return err;
}
