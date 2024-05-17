DEFINE_RUN_ONCE_STATIC(ossl_init_async)
{
#ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_async: async_init()\n");
#endif
    if (!async_init())
        return 0;
    async_inited = 1;
    return 1;
}
