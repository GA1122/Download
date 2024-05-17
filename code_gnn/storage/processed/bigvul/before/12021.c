static int win32atexit(void)
{
    OPENSSL_cleanup();
    return 0;
}
