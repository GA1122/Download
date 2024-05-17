DEFINE_RUN_ONCE_STATIC(ossl_init_engine_padlock)
{
#   ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_engine_padlock: "
                    "engine_load_padlock_int()\n");
#   endif
    engine_load_padlock_int();
    return 1;
}
