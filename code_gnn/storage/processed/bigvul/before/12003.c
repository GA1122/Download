DEFINE_RUN_ONCE_STATIC(ossl_init_engine_capi)
{
#   ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_engine_capi: "
                    "engine_load_capi_int()\n");
#   endif
    engine_load_capi_int();
    return 1;
}
