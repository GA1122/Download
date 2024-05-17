static int aes_wrap_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out,
                           const unsigned char *in, size_t inlen)
{
    EVP_AES_WRAP_CTX *wctx = EVP_C_DATA(EVP_AES_WRAP_CTX,ctx);
    size_t rv;
     
    int pad = EVP_CIPHER_CTX_iv_length(ctx) == 4;
     
    if (!in)
        return 0;
     
    if (!inlen)
        return -1;
     
    if (!EVP_CIPHER_CTX_encrypting(ctx) && (inlen < 16 || inlen & 0x7))
        return -1;
     
    if (!pad && inlen & 0x7)
        return -1;
    if (is_partially_overlapping(out, in, inlen)) {
        EVPerr(EVP_F_AES_WRAP_CIPHER, EVP_R_PARTIALLY_OVERLAPPING);
        return 0;
    }
    if (!out) {
        if (EVP_CIPHER_CTX_encrypting(ctx)) {
             
            if (pad)
                inlen = (inlen + 7) / 8 * 8;
             
            return inlen + 8;
        } else {
             
            return inlen - 8;
        }
    }
    if (pad) {
        if (EVP_CIPHER_CTX_encrypting(ctx))
            rv = CRYPTO_128_wrap_pad(&wctx->ks.ks, wctx->iv,
                                     out, in, inlen,
                                     (block128_f) AES_encrypt);
        else
            rv = CRYPTO_128_unwrap_pad(&wctx->ks.ks, wctx->iv,
                                       out, in, inlen,
                                       (block128_f) AES_decrypt);
    } else {
        if (EVP_CIPHER_CTX_encrypting(ctx))
            rv = CRYPTO_128_wrap(&wctx->ks.ks, wctx->iv,
                                 out, in, inlen, (block128_f) AES_encrypt);
        else
            rv = CRYPTO_128_unwrap(&wctx->ks.ks, wctx->iv,
                                   out, in, inlen, (block128_f) AES_decrypt);
    }
    return rv ? (int)rv : -1;
}
