static int ssl_rsa_private_decrypt(CERT *c, int len, unsigned char *from,
                                   unsigned char *to, int padding)
{
    RSA *rsa;
    int i;

    if ((c == NULL) || (c->pkeys[SSL_PKEY_RSA_ENC].privatekey == NULL)) {
        SSLerr(SSL_F_SSL_RSA_PRIVATE_DECRYPT, SSL_R_NO_PRIVATEKEY);
        return (-1);
    }
    if (c->pkeys[SSL_PKEY_RSA_ENC].privatekey->type != EVP_PKEY_RSA) {
        SSLerr(SSL_F_SSL_RSA_PRIVATE_DECRYPT, SSL_R_PUBLIC_KEY_IS_NOT_RSA);
        return (-1);
    }
    rsa = c->pkeys[SSL_PKEY_RSA_ENC].privatekey->pkey.rsa;

     
    i = RSA_private_decrypt(len, from, to, rsa, padding);
    if (i < 0)
        SSLerr(SSL_F_SSL_RSA_PRIVATE_DECRYPT, ERR_R_RSA_LIB);
    return (i);
}
