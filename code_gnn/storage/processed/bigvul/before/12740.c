STACK_OF(SSL_CIPHER) *ssl_bytes_to_cipher_list(SSL *s,
                                               PACKET *cipher_suites,
                                               STACK_OF(SSL_CIPHER) **skp,
                                               int sslv2format, int *al)
{
    const SSL_CIPHER *c;
    STACK_OF(SSL_CIPHER) *sk;
    int n;
     
    unsigned char cipher[SSLV2_CIPHER_LEN];

    s->s3->send_connection_binding = 0;

    n = sslv2format ? SSLV2_CIPHER_LEN : TLS_CIPHER_LEN;

    if (PACKET_remaining(cipher_suites) == 0) {
        SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST, SSL_R_NO_CIPHERS_SPECIFIED);
        *al = SSL_AD_ILLEGAL_PARAMETER;
        return NULL;
    }

    if (PACKET_remaining(cipher_suites) % n != 0) {
        SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST,
               SSL_R_ERROR_IN_RECEIVED_CIPHER_LIST);
        *al = SSL_AD_DECODE_ERROR;
        return NULL;
    }

    if ((skp == NULL) || (*skp == NULL)) {
        sk = sk_SSL_CIPHER_new_null();  
        if (sk == NULL) {
            SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST, ERR_R_MALLOC_FAILURE);
            *al = SSL_AD_INTERNAL_ERROR;
            return NULL;
        }
    } else {
        sk = *skp;
        sk_SSL_CIPHER_zero(sk);
    }

    if (!PACKET_memdup(cipher_suites, &s->s3->tmp.ciphers_raw,
                       &s->s3->tmp.ciphers_rawlen)) {
        *al = SSL_AD_INTERNAL_ERROR;
        goto err;
    }

    while (PACKET_copy_bytes(cipher_suites, cipher, n)) {
         
        if (sslv2format && cipher[0] != '\0')
            continue;

         
        if ((cipher[n - 2] == ((SSL3_CK_SCSV >> 8) & 0xff)) &&
            (cipher[n - 1] == (SSL3_CK_SCSV & 0xff))) {
             
            if (s->renegotiate) {
                SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST,
                       SSL_R_SCSV_RECEIVED_WHEN_RENEGOTIATING);
                *al = SSL_AD_HANDSHAKE_FAILURE;
                goto err;
            }
            s->s3->send_connection_binding = 1;
            continue;
        }

         
        if ((cipher[n - 2] == ((SSL3_CK_FALLBACK_SCSV >> 8) & 0xff)) &&
            (cipher[n - 1] == (SSL3_CK_FALLBACK_SCSV & 0xff))) {
             
            if (!ssl_check_version_downgrade(s)) {
                SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST,
                       SSL_R_INAPPROPRIATE_FALLBACK);
                *al = SSL_AD_INAPPROPRIATE_FALLBACK;
                goto err;
            }
            continue;
        }

         
        c = ssl_get_cipher_by_char(s, sslv2format ? &cipher[1] : cipher);
        if (c != NULL) {
            if (!sk_SSL_CIPHER_push(sk, c)) {
                SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST, ERR_R_MALLOC_FAILURE);
                *al = SSL_AD_INTERNAL_ERROR;
                goto err;
            }
        }
    }
    if (PACKET_remaining(cipher_suites) > 0) {
        *al = SSL_AD_INTERNAL_ERROR;
        SSLerr(SSL_F_SSL_BYTES_TO_CIPHER_LIST, ERR_R_INTERNAL_ERROR);
        goto err;
    }

    if (skp != NULL)
        *skp = sk;
    return (sk);
 err:
    if ((skp == NULL) || (*skp == NULL))
        sk_SSL_CIPHER_free(sk);
    return NULL;
}
