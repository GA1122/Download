DEFINE_RUN_ONCE_STATIC(ossl_init_register_atexit)
{
#ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_register_atexit()\n");
#endif
#ifndef OPENSSL_SYS_UEFI
# ifdef _WIN32
     
    if (_onexit(win32atexit) == NULL)
        return 0;
# else
    if (atexit(OPENSSL_cleanup) != 0)
        return 0;
# endif
#endif

    return 1;
}
