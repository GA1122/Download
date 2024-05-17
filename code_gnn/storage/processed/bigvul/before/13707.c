long ssl_get_algorithm2(SSL *s)
{
    long alg2 = s->s3->tmp.new_cipher->algorithm2;
    if (s->method->ssl3_enc->enc_flags & SSL_ENC_FLAG_SHA256_PRF
        && alg2 == (SSL_HANDSHAKE_MAC_DEFAULT | TLS1_PRF))
        return SSL_HANDSHAKE_MAC_SHA256 | TLS1_PRF_SHA256;
    return alg2;
}