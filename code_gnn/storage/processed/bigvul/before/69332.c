static int aes_ocb_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
                            const unsigned char *iv, int enc)
{
    EVP_AES_OCB_CTX *octx = EVP_C_DATA(EVP_AES_OCB_CTX,ctx);
    if (!iv && !key)
        return 1;
    if (key) {
        do {
             
# ifdef HWAES_CAPABLE
            if (HWAES_CAPABLE) {
                HWAES_set_encrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 8,
                                      &octx->ksenc.ks);
                HWAES_set_decrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 8,
                                      &octx->ksdec.ks);
                if (!CRYPTO_ocb128_init(&octx->ocb,
                                        &octx->ksenc.ks, &octx->ksdec.ks,
                                        (block128_f) HWAES_encrypt,
                                        (block128_f) HWAES_decrypt,
                                        enc ? HWAES_ocb_encrypt
                                            : HWAES_ocb_decrypt))
                    return 0;
                break;
            }
# endif
# ifdef VPAES_CAPABLE
            if (VPAES_CAPABLE) {
                vpaes_set_encrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 8,
                                      &octx->ksenc.ks);
                vpaes_set_decrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 8,
                                      &octx->ksdec.ks);
                if (!CRYPTO_ocb128_init(&octx->ocb,
                                        &octx->ksenc.ks, &octx->ksdec.ks,
                                        (block128_f) vpaes_encrypt,
                                        (block128_f) vpaes_decrypt,
                                        NULL))
                    return 0;
                break;
            }
# endif
            AES_set_encrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 8,
                                &octx->ksenc.ks);
            AES_set_decrypt_key(key, EVP_CIPHER_CTX_key_length(ctx) * 8,
                                &octx->ksdec.ks);
            if (!CRYPTO_ocb128_init(&octx->ocb,
                                    &octx->ksenc.ks, &octx->ksdec.ks,
                                    (block128_f) AES_encrypt,
                                    (block128_f) AES_decrypt,
                                    NULL))
                return 0;
        }
        while (0);

         
        if (iv == NULL && octx->iv_set)
            iv = octx->iv;
        if (iv) {
            if (CRYPTO_ocb128_setiv(&octx->ocb, iv, octx->ivlen, octx->taglen)
                != 1)
                return 0;
            octx->iv_set = 1;
        }
        octx->key_set = 1;
    } else {
         
        if (octx->key_set)
            CRYPTO_ocb128_setiv(&octx->ocb, iv, octx->ivlen, octx->taglen);
        else
            memcpy(octx->iv, iv, octx->ivlen);
        octx->iv_set = 1;
    }
    return 1;
}