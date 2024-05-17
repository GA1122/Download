int xmp_get_error()
{
#if HAVE_NATIVE_TLS
    return g_error;
#else
    return get_error_for_thread();
#endif
}
