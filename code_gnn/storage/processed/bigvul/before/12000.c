DEFINE_RUN_ONCE_STATIC(ossl_init_engine_rdrand)
{
#  ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_engine_rdrand: "
                    "engine_load_rdrand_int()\n");
#  endif
    engine_load_rdrand_int();
    return 1;
}
