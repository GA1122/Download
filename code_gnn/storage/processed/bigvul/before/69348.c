static int aesni_xts_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
                              const unsigned char *iv, int enc)
{
    EVP_AES_XTS_CTX *xctx = EVP_C_DATA(EVP_AES_XTS_CTX,ctx);
    if (!iv && !key)
        return 1;

    if (key) {
         
        if (enc) {
            aesni_set_encrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 4,
                                  &xctx->ks1.ks);
            xctx->xts.block1 = (block128_f) aesni_encrypt;
            xctx->stream = aesni_xts_encrypt;
        } else {
            aesni_set_decrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 4,
                                  &xctx->ks1.ks);
            xctx->xts.block1 = (block128_f) aesni_decrypt;
            xctx->stream = aesni_xts_decrypt;
        }

        aesni_set_encrypt_key(key + EVP_CIPHER_CTX_key_length(ctx) / 2,
                              EVP_CIPHER_CTX_key_length(ctx) * 4,
                              &xctx->ks2.ks);
        xctx->xts.block2 = (block128_f) aesni_encrypt;

        xctx->xts.key1 = &xctx->ks1;
    }

    if (iv) {
        xctx->xts.key2 = &xctx->ks2;
        memcpy(EVP_CIPHER_CTX_iv_noconst(ctx), iv, 16);
    }

    return 1;
}