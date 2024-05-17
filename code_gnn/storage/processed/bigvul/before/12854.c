static size_t tls1_1_multi_block_encrypt(EVP_AES_HMAC_SHA1 *key,
                                         unsigned char *out,
                                         const unsigned char *inp,
                                         size_t inp_len, int n4x)
{                                
    HASH_DESC hash_d[8], edges[8];
    CIPH_DESC ciph_d[8];
    unsigned char storage[sizeof(SHA1_MB_CTX) + 32];
    union {
        u64 q[16];
        u32 d[32];
        u8 c[128];
    } blocks[8];
    SHA1_MB_CTX *ctx;
    unsigned int frag, last, packlen, i, x4 = 4 * n4x, minblocks, processed =
        0;
    size_t ret = 0;
    u8 *IVs;
#   if defined(BSWAP8)
    u64 seqnum;
#   endif

     
    if (RAND_bytes((IVs = blocks[0].c), 16 * x4) <= 0)
        return 0;

    ctx = (SHA1_MB_CTX *) (storage + 32 - ((size_t)storage % 32));  

    frag = (unsigned int)inp_len >> (1 + n4x);
    last = (unsigned int)inp_len + frag - (frag << (1 + n4x));
    if (last > frag && ((last + 13 + 9) % 64) < (x4 - 1)) {
        frag++;
        last -= x4 - 1;
    }

    packlen = 5 + 16 + ((frag + 20 + 16) & -16);

     
    hash_d[0].ptr = inp;
    ciph_d[0].inp = inp;
     
    ciph_d[0].out = out + 5 + 16;
    memcpy(ciph_d[0].out - 16, IVs, 16);
    memcpy(ciph_d[0].iv, IVs, 16);
    IVs += 16;

    for (i = 1; i < x4; i++) {
        ciph_d[i].inp = hash_d[i].ptr = hash_d[i - 1].ptr + frag;
        ciph_d[i].out = ciph_d[i - 1].out + packlen;
        memcpy(ciph_d[i].out - 16, IVs, 16);
        memcpy(ciph_d[i].iv, IVs, 16);
        IVs += 16;
    }

#   if defined(BSWAP8)
    memcpy(blocks[0].c, key->md.data, 8);
    seqnum = BSWAP8(blocks[0].q[0]);
#   endif
    for (i = 0; i < x4; i++) {
        unsigned int len = (i == (x4 - 1) ? last : frag);
#   if !defined(BSWAP8)
        unsigned int carry, j;
#   endif

        ctx->A[i] = key->md.h0;
        ctx->B[i] = key->md.h1;
        ctx->C[i] = key->md.h2;
        ctx->D[i] = key->md.h3;
        ctx->E[i] = key->md.h4;

         
#   if defined(BSWAP8)
        blocks[i].q[0] = BSWAP8(seqnum + i);
#   else
        for (carry = i, j = 8; j--;) {
            blocks[i].c[j] = ((u8 *)key->md.data)[j] + carry;
            carry = (blocks[i].c[j] - carry) >> (sizeof(carry) * 8 - 1);
        }
#   endif
        blocks[i].c[8] = ((u8 *)key->md.data)[8];
        blocks[i].c[9] = ((u8 *)key->md.data)[9];
        blocks[i].c[10] = ((u8 *)key->md.data)[10];
         
        blocks[i].c[11] = (u8)(len >> 8);
        blocks[i].c[12] = (u8)(len);

        memcpy(blocks[i].c + 13, hash_d[i].ptr, 64 - 13);
        hash_d[i].ptr += 64 - 13;
        hash_d[i].blocks = (len - (64 - 13)) / 64;

        edges[i].ptr = blocks[i].c;
        edges[i].blocks = 1;
    }

     
    sha1_multi_block(ctx, edges, n4x);
     
#   define MAXCHUNKSIZE    2048
#   if     MAXCHUNKSIZE%64
#    error  "MAXCHUNKSIZE is not divisible by 64"
#   elif   MAXCHUNKSIZE
     
    minblocks = ((frag <= last ? frag : last) - (64 - 13)) / 64;
    if (minblocks > MAXCHUNKSIZE / 64) {
        for (i = 0; i < x4; i++) {
            edges[i].ptr = hash_d[i].ptr;
            edges[i].blocks = MAXCHUNKSIZE / 64;
            ciph_d[i].blocks = MAXCHUNKSIZE / 16;
        }
        do {
            sha1_multi_block(ctx, edges, n4x);
            aesni_multi_cbc_encrypt(ciph_d, &key->ks, n4x);

            for (i = 0; i < x4; i++) {
                edges[i].ptr = hash_d[i].ptr += MAXCHUNKSIZE;
                hash_d[i].blocks -= MAXCHUNKSIZE / 64;
                edges[i].blocks = MAXCHUNKSIZE / 64;
                ciph_d[i].inp += MAXCHUNKSIZE;
                ciph_d[i].out += MAXCHUNKSIZE;
                ciph_d[i].blocks = MAXCHUNKSIZE / 16;
                memcpy(ciph_d[i].iv, ciph_d[i].out - 16, 16);
            }
            processed += MAXCHUNKSIZE;
            minblocks -= MAXCHUNKSIZE / 64;
        } while (minblocks > MAXCHUNKSIZE / 64);
    }
#   endif
#   undef  MAXCHUNKSIZE
    sha1_multi_block(ctx, hash_d, n4x);

    memset(blocks, 0, sizeof(blocks));
    for (i = 0; i < x4; i++) {
        unsigned int len = (i == (x4 - 1) ? last : frag),
            off = hash_d[i].blocks * 64;
        const unsigned char *ptr = hash_d[i].ptr + off;

        off = (len - processed) - (64 - 13) - off;  
        memcpy(blocks[i].c, ptr, off);
        blocks[i].c[off] = 0x80;
        len += 64 + 13;          
        len *= 8;                
        if (off < (64 - 8)) {
#   ifdef BSWAP4
            blocks[i].d[15] = BSWAP4(len);
#   else
            PUTU32(blocks[i].c + 60, len);
#   endif
            edges[i].blocks = 1;
        } else {
#   ifdef BSWAP4
            blocks[i].d[31] = BSWAP4(len);
#   else
            PUTU32(blocks[i].c + 124, len);
#   endif
            edges[i].blocks = 2;
        }
        edges[i].ptr = blocks[i].c;
    }

     
    sha1_multi_block(ctx, edges, n4x);

    memset(blocks, 0, sizeof(blocks));
    for (i = 0; i < x4; i++) {
#   ifdef BSWAP4
        blocks[i].d[0] = BSWAP4(ctx->A[i]);
        ctx->A[i] = key->tail.h0;
        blocks[i].d[1] = BSWAP4(ctx->B[i]);
        ctx->B[i] = key->tail.h1;
        blocks[i].d[2] = BSWAP4(ctx->C[i]);
        ctx->C[i] = key->tail.h2;
        blocks[i].d[3] = BSWAP4(ctx->D[i]);
        ctx->D[i] = key->tail.h3;
        blocks[i].d[4] = BSWAP4(ctx->E[i]);
        ctx->E[i] = key->tail.h4;
        blocks[i].c[20] = 0x80;
        blocks[i].d[15] = BSWAP4((64 + 20) * 8);
#   else
        PUTU32(blocks[i].c + 0, ctx->A[i]);
        ctx->A[i] = key->tail.h0;
        PUTU32(blocks[i].c + 4, ctx->B[i]);
        ctx->B[i] = key->tail.h1;
        PUTU32(blocks[i].c + 8, ctx->C[i]);
        ctx->C[i] = key->tail.h2;
        PUTU32(blocks[i].c + 12, ctx->D[i]);
        ctx->D[i] = key->tail.h3;
        PUTU32(blocks[i].c + 16, ctx->E[i]);
        ctx->E[i] = key->tail.h4;
        blocks[i].c[20] = 0x80;
        PUTU32(blocks[i].c + 60, (64 + 20) * 8);
#   endif
        edges[i].ptr = blocks[i].c;
        edges[i].blocks = 1;
    }

     
    sha1_multi_block(ctx, edges, n4x);

    for (i = 0; i < x4; i++) {
        unsigned int len = (i == (x4 - 1) ? last : frag), pad, j;
        unsigned char *out0 = out;

        memcpy(ciph_d[i].out, ciph_d[i].inp, len - processed);
        ciph_d[i].inp = ciph_d[i].out;

        out += 5 + 16 + len;

         
        PUTU32(out + 0, ctx->A[i]);
        PUTU32(out + 4, ctx->B[i]);
        PUTU32(out + 8, ctx->C[i]);
        PUTU32(out + 12, ctx->D[i]);
        PUTU32(out + 16, ctx->E[i]);
        out += 20;
        len += 20;

         
        pad = 15 - len % 16;
        for (j = 0; j <= pad; j++)
            *(out++) = pad;
        len += pad + 1;

        ciph_d[i].blocks = (len - processed) / 16;
        len += 16;               

         
        out0[0] = ((u8 *)key->md.data)[8];
        out0[1] = ((u8 *)key->md.data)[9];
        out0[2] = ((u8 *)key->md.data)[10];
        out0[3] = (u8)(len >> 8);
        out0[4] = (u8)(len);

        ret += len + 5;
        inp += frag;
    }

    aesni_multi_cbc_encrypt(ciph_d, &key->ks, n4x);

    OPENSSL_cleanse(blocks, sizeof(blocks));
    OPENSSL_cleanse(ctx, sizeof(*ctx));

    return ret;
}
