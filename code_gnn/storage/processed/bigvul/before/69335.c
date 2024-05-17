static int aes_t4_gcm_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
                               const unsigned char *iv, int enc)
{
    EVP_AES_GCM_CTX *gctx = EVP_C_DATA(EVP_AES_GCM_CTX,ctx);
    if (!iv && !key)
        return 1;
    if (key) {
        int bits = EVP_CIPHER_CTX_key_length(ctx) * 8;
        aes_t4_set_encrypt_key(key, bits, &gctx->ks.ks);
        CRYPTO_gcm128_init(&gctx->gcm, &gctx->ks,
                           (block128_f) aes_t4_encrypt);
        switch (bits) {
        case 128:
            gctx->ctr = (ctr128_f) aes128_t4_ctr32_encrypt;
            break;
        case 192:
            gctx->ctr = (ctr128_f) aes192_t4_ctr32_encrypt;
            break;
        case 256:
            gctx->ctr = (ctr128_f) aes256_t4_ctr32_encrypt;
            break;
        default:
            return 0;
        }
         
        if (iv == NULL && gctx->iv_set)
            iv = gctx->iv;
        if (iv) {
            CRYPTO_gcm128_setiv(&gctx->gcm, iv, gctx->ivlen);
            gctx->iv_set = 1;
        }
        gctx->key_set = 1;
    } else {
         
        if (gctx->key_set)
            CRYPTO_gcm128_setiv(&gctx->gcm, iv, gctx->ivlen);
        else
            memcpy(gctx->iv, iv, gctx->ivlen);
        gctx->iv_set = 1;
        gctx->iv_gen = 0;
    }
    return 1;
}