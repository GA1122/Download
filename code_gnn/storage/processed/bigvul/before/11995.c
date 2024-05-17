DEFINE_RUN_ONCE_STATIC(ossl_init_add_all_digests)
{
     
#ifndef OPENSSL_NO_AUTOALGINIT
# ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_add_all_digests: "
                    "openssl_add_all_digests()\n");
# endif
    openssl_add_all_digests_int();
#endif
    return 1;
}
