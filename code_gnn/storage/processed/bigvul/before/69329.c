static int aes_ocb_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out,
                          const unsigned char *in, size_t len)
{
    unsigned char *buf;
    int *buf_len;
    int written_len = 0;
    size_t trailing_len;
    EVP_AES_OCB_CTX *octx = EVP_C_DATA(EVP_AES_OCB_CTX,ctx);

     
    if (!octx->iv_set)
        return -1;

    if (!octx->key_set)
        return -1;

    if (in != NULL) {
         

         
        if (out == NULL) {
            buf = octx->aad_buf;
            buf_len = &(octx->aad_buf_len);
        } else {
            buf = octx->data_buf;
            buf_len = &(octx->data_buf_len);

            if (is_partially_overlapping(out + *buf_len, in, len)) {
                EVPerr(EVP_F_AES_OCB_CIPHER, EVP_R_PARTIALLY_OVERLAPPING);
                return 0;
            }
        }

         
        if (*buf_len > 0) {
            unsigned int remaining;

            remaining = AES_BLOCK_SIZE - (*buf_len);
            if (remaining > len) {
                memcpy(buf + (*buf_len), in, len);
                *(buf_len) += len;
                return 0;
            }
            memcpy(buf + (*buf_len), in, remaining);

             
            len -= remaining;
            in += remaining;
            if (out == NULL) {
                if (!CRYPTO_ocb128_aad(&octx->ocb, buf, AES_BLOCK_SIZE))
                    return -1;
            } else if (EVP_CIPHER_CTX_encrypting(ctx)) {
                if (!CRYPTO_ocb128_encrypt(&octx->ocb, buf, out,
                                           AES_BLOCK_SIZE))
                    return -1;
            } else {
                if (!CRYPTO_ocb128_decrypt(&octx->ocb, buf, out,
                                           AES_BLOCK_SIZE))
                    return -1;
            }
            written_len = AES_BLOCK_SIZE;
            *buf_len = 0;
            if (out != NULL)
                out += AES_BLOCK_SIZE;
        }

         
        trailing_len = len % AES_BLOCK_SIZE;

         
        if (len != trailing_len) {
            if (out == NULL) {
                if (!CRYPTO_ocb128_aad(&octx->ocb, in, len - trailing_len))
                    return -1;
            } else if (EVP_CIPHER_CTX_encrypting(ctx)) {
                if (!CRYPTO_ocb128_encrypt
                    (&octx->ocb, in, out, len - trailing_len))
                    return -1;
            } else {
                if (!CRYPTO_ocb128_decrypt
                    (&octx->ocb, in, out, len - trailing_len))
                    return -1;
            }
            written_len += len - trailing_len;
            in += len - trailing_len;
        }

         
        if (trailing_len > 0) {
            memcpy(buf, in, trailing_len);
            *buf_len = trailing_len;
        }

        return written_len;
    } else {
         
        if (octx->data_buf_len > 0) {
            if (EVP_CIPHER_CTX_encrypting(ctx)) {
                if (!CRYPTO_ocb128_encrypt(&octx->ocb, octx->data_buf, out,
                                           octx->data_buf_len))
                    return -1;
            } else {
                if (!CRYPTO_ocb128_decrypt(&octx->ocb, octx->data_buf, out,
                                           octx->data_buf_len))
                    return -1;
            }
            written_len = octx->data_buf_len;
            octx->data_buf_len = 0;
        }
        if (octx->aad_buf_len > 0) {
            if (!CRYPTO_ocb128_aad
                (&octx->ocb, octx->aad_buf, octx->aad_buf_len))
                return -1;
            octx->aad_buf_len = 0;
        }
         
        if (!EVP_CIPHER_CTX_encrypting(ctx)) {
            if (octx->taglen < 0)
                return -1;
            if (CRYPTO_ocb128_finish(&octx->ocb,
                                     octx->tag, octx->taglen) != 0)
                return -1;
            octx->iv_set = 0;
            return written_len;
        }
         
        if (CRYPTO_ocb128_tag(&octx->ocb, octx->tag, 16) != 1)
            return -1;
         
        octx->iv_set = 0;
        return written_len;
    }
}