DEFINE_RUN_ONCE_STATIC(ossl_init_engine_dynamic)
{
# ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_engine_dynamic: "
                    "engine_load_dynamic_int()\n");
# endif
    engine_load_dynamic_int();
    return 1;
}
