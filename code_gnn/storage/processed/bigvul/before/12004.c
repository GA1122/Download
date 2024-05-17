DEFINE_RUN_ONCE_STATIC(ossl_init_engine_afalg)
{
#   ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_engine_afalg: "
                    "engine_load_afalg_int()\n");
#   endif
    engine_load_afalg_int();
    return 1;
}
