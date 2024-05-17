int Condor_Auth_SSL :: init_OpenSSL(void)
{
    if (!SSL_library_init()) {
        return AUTH_SSL_ERROR;
    }
    SSL_load_error_strings();
    return AUTH_SSL_A_OK;
}
