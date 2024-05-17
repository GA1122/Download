DEFINE_RUN_ONCE_STATIC(ossl_init_engine_devcrypto)
{
#  ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_engine_devcrypto: "
                    "engine_load_devcrypto_int()\n");
#  endif
    engine_load_devcrypto_int();
    return 1;
}
