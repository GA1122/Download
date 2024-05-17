int tls1_cbc_remove_padding(const SSL *s,
                            SSL3_RECORD *rec,
                            unsigned block_size, unsigned mac_size)
{
    unsigned padding_length, good, to_check, i;
    const unsigned overhead = 1    + mac_size;
     
    if (SSL_USE_EXPLICIT_IV(s)) {
         
        if (overhead + block_size > rec->length)
            return 0;
         
        rec->data += block_size;
        rec->input += block_size;
        rec->length -= block_size;
        rec->orig_len -= block_size;
    } else if (overhead > rec->length)
        return 0;

    padding_length = rec->data[rec->length - 1];

    if (EVP_CIPHER_flags(EVP_CIPHER_CTX_cipher(s->enc_read_ctx)) &
        EVP_CIPH_FLAG_AEAD_CIPHER) {
         
        rec->length -= padding_length + 1;
        return 1;
    }

    good = constant_time_ge(rec->length, overhead + padding_length);
     
    to_check = 256;             
    if (to_check > rec->length)
        to_check = rec->length;

    for (i = 0; i < to_check; i++) {
        unsigned char mask = constant_time_ge_8(padding_length, i);
        unsigned char b = rec->data[rec->length - 1 - i];
         
        good &= ~(mask & (padding_length ^ b));
    }

     
    good = constant_time_eq(0xff, good & 0xff);
    rec->length -= good & (padding_length + 1);

    return constant_time_select_int(good, 1, -1);
}
