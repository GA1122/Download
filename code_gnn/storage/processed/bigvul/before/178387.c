 static int aesni_cbc_hmac_sha256_cipher(EVP_CIPHER_CTX *ctx,
                                         unsigned char *out,
                                         const unsigned char *in, size_t len)
 {
     EVP_AES_HMAC_SHA256 *key = data(ctx);
     unsigned int l;
     size_t plen = key->payload_length, iv = 0,  
         sha_off = 0;
 #  if defined(STITCHED_CALL)
     size_t aes_off = 0, blocks;
 
     sha_off = SHA256_CBLOCK - key->md.num;
 #  endif
 
     key->payload_length = NO_PAYLOAD_LENGTH;
 
     if (len % AES_BLOCK_SIZE)
         return 0;
 
     if (ctx->encrypt) {
         if (plen == NO_PAYLOAD_LENGTH)
             plen = len;
         else if (len !=
                  ((plen + SHA256_DIGEST_LENGTH +
                    AES_BLOCK_SIZE) & -AES_BLOCK_SIZE))
             return 0;
         else if (key->aux.tls_ver >= TLS1_1_VERSION)
             iv = AES_BLOCK_SIZE;
 
 #  if defined(STITCHED_CALL)
          
         if (OPENSSL_ia32cap_P[1] & (1 << (60 - 32)) &&  
             ((OPENSSL_ia32cap_P[1] & (1 << (43 - 32)))  
              | (OPENSSL_ia32cap_P[0] & (1<<30))) &&     
             plen > (sha_off + iv) &&
             (blocks = (plen - (sha_off + iv)) / SHA256_CBLOCK)) {
             SHA256_Update(&key->md, in + iv, sha_off);
 
             (void)aesni_cbc_sha256_enc(in, out, blocks, &key->ks,
                                        ctx->iv, &key->md, in + iv + sha_off);
             blocks *= SHA256_CBLOCK;
             aes_off += blocks;
             sha_off += blocks;
             key->md.Nh += blocks >> 29;
             key->md.Nl += blocks <<= 3;
             if (key->md.Nl < (unsigned int)blocks)
                 key->md.Nh++;
         } else {
             sha_off = 0;
         }
 #  endif
         sha_off += iv;
         SHA256_Update(&key->md, in + sha_off, plen - sha_off);
 
         if (plen != len) {       
             if (in != out)
                 memcpy(out + aes_off, in + aes_off, plen - aes_off);
 
              
             SHA256_Final(out + plen, &key->md);
             key->md = key->tail;
             SHA256_Update(&key->md, out + plen, SHA256_DIGEST_LENGTH);
             SHA256_Final(out + plen, &key->md);
 
              
             plen += SHA256_DIGEST_LENGTH;
             for (l = len - plen - 1; plen < len; plen++)
                 out[plen] = l;
              
             aesni_cbc_encrypt(out + aes_off, out + aes_off, len - aes_off,
                               &key->ks, ctx->iv, 1);
         } else {
             aesni_cbc_encrypt(in + aes_off, out + aes_off, len - aes_off,
                               &key->ks, ctx->iv, 1);
         }
     } else {
         union {
             unsigned int u[SHA256_DIGEST_LENGTH / sizeof(unsigned int)];
             unsigned char c[64 + SHA256_DIGEST_LENGTH];
         } mac, *pmac;
 
          
         pmac = (void *)(((size_t)mac.c + 63) & ((size_t)0 - 64));
 
          
         aesni_cbc_encrypt(in, out, len, &key->ks, ctx->iv, 0);
 
         if (plen != NO_PAYLOAD_LENGTH) {  
             size_t inp_len, mask, j, i;
             unsigned int res, maxpad, pad, bitlen;
             int ret = 1;
             union {
                 unsigned int u[SHA_LBLOCK];
                 unsigned char c[SHA256_CBLOCK];
             } *data = (void *)key->md.data;
 
             if ((key->aux.tls_aad[plen - 4] << 8 | key->aux.tls_aad[plen - 3])
                 >= TLS1_1_VERSION)
                 iv = AES_BLOCK_SIZE;
 
             if (len < (iv + SHA256_DIGEST_LENGTH + 1))
                 return 0;
 
              
             out += iv;
             len -= iv;
 
              
             pad = out[len - 1];
             maxpad = len - (SHA256_DIGEST_LENGTH + 1);
             maxpad |= (255 - maxpad) >> (sizeof(maxpad) * 8 - 8);
              maxpad |= (255 - maxpad) >> (sizeof(maxpad) * 8 - 8);
              maxpad &= 255;
  
//             ret &= constant_time_ge(maxpad, pad);
// 
              inp_len = len - (SHA256_DIGEST_LENGTH + pad + 1);
              mask = (0 - ((inp_len - len) >> (sizeof(inp_len) * 8 - 1)));
              inp_len &= mask;
             key->aux.tls_aad[plen - 1] = inp_len;
 
              
             key->md = key->head;
             SHA256_Update(&key->md, key->aux.tls_aad, plen);
 
 #  if 1
             len -= SHA256_DIGEST_LENGTH;  
             if (len >= (256 + SHA256_CBLOCK)) {
                 j = (len - (256 + SHA256_CBLOCK)) & (0 - SHA256_CBLOCK);
                 j += SHA256_CBLOCK - key->md.num;
                 SHA256_Update(&key->md, out, j);
                 out += j;
                 len -= j;
                 inp_len -= j;
             }
 
              
             bitlen = key->md.Nl + (inp_len << 3);  
 #   ifdef BSWAP4
             bitlen = BSWAP4(bitlen);
 #   else
             mac.c[0] = 0;
             mac.c[1] = (unsigned char)(bitlen >> 16);
             mac.c[2] = (unsigned char)(bitlen >> 8);
             mac.c[3] = (unsigned char)bitlen;
             bitlen = mac.u[0];
 #   endif
 
             pmac->u[0] = 0;
             pmac->u[1] = 0;
             pmac->u[2] = 0;
             pmac->u[3] = 0;
             pmac->u[4] = 0;
             pmac->u[5] = 0;
             pmac->u[6] = 0;
             pmac->u[7] = 0;
 
             for (res = key->md.num, j = 0; j < len; j++) {
                 size_t c = out[j];
                 mask = (j - inp_len) >> (sizeof(j) * 8 - 8);
                 c &= mask;
                 c |= 0x80 & ~mask & ~((inp_len - j) >> (sizeof(j) * 8 - 8));
                 data->c[res++] = (unsigned char)c;
 
                 if (res != SHA256_CBLOCK)
                     continue;
 
                  
                 mask = 0 - ((inp_len + 7 - j) >> (sizeof(j) * 8 - 1));
                 data->u[SHA_LBLOCK - 1] |= bitlen & mask;
                 sha256_block_data_order(&key->md, data, 1);
                 mask &= 0 - ((j - inp_len - 72) >> (sizeof(j) * 8 - 1));
                 pmac->u[0] |= key->md.h[0] & mask;
                 pmac->u[1] |= key->md.h[1] & mask;
                 pmac->u[2] |= key->md.h[2] & mask;
                 pmac->u[3] |= key->md.h[3] & mask;
                 pmac->u[4] |= key->md.h[4] & mask;
                 pmac->u[5] |= key->md.h[5] & mask;
                 pmac->u[6] |= key->md.h[6] & mask;
                 pmac->u[7] |= key->md.h[7] & mask;
                 res = 0;
             }
 
             for (i = res; i < SHA256_CBLOCK; i++, j++)
                 data->c[i] = 0;
 
             if (res > SHA256_CBLOCK - 8) {
                 mask = 0 - ((inp_len + 8 - j) >> (sizeof(j) * 8 - 1));
                 data->u[SHA_LBLOCK - 1] |= bitlen & mask;
                 sha256_block_data_order(&key->md, data, 1);
                 mask &= 0 - ((j - inp_len - 73) >> (sizeof(j) * 8 - 1));
                 pmac->u[0] |= key->md.h[0] & mask;
                 pmac->u[1] |= key->md.h[1] & mask;
                 pmac->u[2] |= key->md.h[2] & mask;
                 pmac->u[3] |= key->md.h[3] & mask;
                 pmac->u[4] |= key->md.h[4] & mask;
                 pmac->u[5] |= key->md.h[5] & mask;
                 pmac->u[6] |= key->md.h[6] & mask;
                 pmac->u[7] |= key->md.h[7] & mask;
 
                 memset(data, 0, SHA256_CBLOCK);
                 j += 64;
             }
             data->u[SHA_LBLOCK - 1] = bitlen;
             sha256_block_data_order(&key->md, data, 1);
             mask = 0 - ((j - inp_len - 73) >> (sizeof(j) * 8 - 1));
             pmac->u[0] |= key->md.h[0] & mask;
             pmac->u[1] |= key->md.h[1] & mask;
             pmac->u[2] |= key->md.h[2] & mask;
             pmac->u[3] |= key->md.h[3] & mask;
             pmac->u[4] |= key->md.h[4] & mask;
             pmac->u[5] |= key->md.h[5] & mask;
             pmac->u[6] |= key->md.h[6] & mask;
             pmac->u[7] |= key->md.h[7] & mask;
 
 #   ifdef BSWAP4
             pmac->u[0] = BSWAP4(pmac->u[0]);
             pmac->u[1] = BSWAP4(pmac->u[1]);
             pmac->u[2] = BSWAP4(pmac->u[2]);
             pmac->u[3] = BSWAP4(pmac->u[3]);
             pmac->u[4] = BSWAP4(pmac->u[4]);
             pmac->u[5] = BSWAP4(pmac->u[5]);
             pmac->u[6] = BSWAP4(pmac->u[6]);
             pmac->u[7] = BSWAP4(pmac->u[7]);
 #   else
             for (i = 0; i < 8; i++) {
                 res = pmac->u[i];
                 pmac->c[4 * i + 0] = (unsigned char)(res >> 24);
                 pmac->c[4 * i + 1] = (unsigned char)(res >> 16);
                 pmac->c[4 * i + 2] = (unsigned char)(res >> 8);
                 pmac->c[4 * i + 3] = (unsigned char)res;
             }
 #   endif
             len += SHA256_DIGEST_LENGTH;
 #  else
             SHA256_Update(&key->md, out, inp_len);
             res = key->md.num;
             SHA256_Final(pmac->c, &key->md);
 
             {
                 unsigned int inp_blocks, pad_blocks;
 
                  
                 inp_blocks =
                     1 + ((SHA256_CBLOCK - 9 - res) >> (sizeof(res) * 8 - 1));
                 res += (unsigned int)(len - inp_len);
                 pad_blocks = res / SHA256_CBLOCK;
                 res %= SHA256_CBLOCK;
                 pad_blocks +=
                     1 + ((SHA256_CBLOCK - 9 - res) >> (sizeof(res) * 8 - 1));
                 for (; inp_blocks < pad_blocks; inp_blocks++)
                     sha1_block_data_order(&key->md, data, 1);
             }
 #  endif
             key->md = key->tail;
             SHA256_Update(&key->md, pmac->c, SHA256_DIGEST_LENGTH);
             SHA256_Final(pmac->c, &key->md);
 
              
             out += inp_len;
             len -= inp_len;
 #  if 1
             {
                 unsigned char *p =
                     out + len - 1 - maxpad - SHA256_DIGEST_LENGTH;
                 size_t off = out - p;
                 unsigned int c, cmask;
 
                 maxpad += SHA256_DIGEST_LENGTH;
                 for (res = 0, i = 0, j = 0; j < maxpad; j++) {
                     c = p[j];
                     cmask =
                         ((int)(j - off - SHA256_DIGEST_LENGTH)) >>
                         (sizeof(int) * 8 - 1);
                     res |= (c ^ pad) & ~cmask;  
                     cmask &= ((int)(off - 1 - j)) >> (sizeof(int) * 8 - 1);
                     res |= (c ^ pmac->c[i]) & cmask;
                     i += 1 & cmask;
                 }
                 maxpad -= SHA256_DIGEST_LENGTH;
 
                 res = 0 - ((0 - res) >> (sizeof(res) * 8 - 1));
                 ret &= (int)~res;
             }
 #  else
             for (res = 0, i = 0; i < SHA256_DIGEST_LENGTH; i++)
                 res |= out[i] ^ pmac->c[i];
             res = 0 - ((0 - res) >> (sizeof(res) * 8 - 1));
             ret &= (int)~res;
 
              
             pad = (pad & ~res) | (maxpad & res);
             out = out + len - 1 - pad;
             for (res = 0, i = 0; i < pad; i++)
                 res |= out[i] ^ pad;
 
             res = (0 - res) >> (sizeof(res) * 8 - 1);
             ret &= (int)~res;
 #  endif
             return ret;
         } else {
             SHA256_Update(&key->md, out, len);
         }
     }
 
     return 1;
 }