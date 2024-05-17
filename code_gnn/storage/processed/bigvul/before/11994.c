DEFINE_RUN_ONCE_STATIC(ossl_init_add_all_ciphers)
{
     
#ifndef OPENSSL_NO_AUTOALGINIT
# ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_add_all_ciphers: "
                    "openssl_add_all_ciphers_int()\n");
# endif
    openssl_add_all_ciphers_int();
#endif
    return 1;
}
