DEFINE_RUN_ONCE_STATIC(ossl_init_load_crypto_strings)
{
    int ret = 1;
     
#if !defined(OPENSSL_NO_ERR) && !defined(OPENSSL_NO_AUTOERRINIT)
# ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_load_crypto_strings: "
                    "err_load_crypto_strings_int()\n");
# endif
    ret = err_load_crypto_strings_int();
    load_crypto_strings_inited = 1;
#endif
    return ret;
}
