pkinit_openssl_init()
{
     
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    return 0;
}
