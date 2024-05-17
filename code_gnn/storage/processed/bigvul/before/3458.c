network_end ()
{
#ifdef HAVE_GNUTLS
    if (network_init_ok)
    {
        network_init_ok = 0;
        gnutls_certificate_free_credentials (gnutls_xcred);
        gnutls_global_deinit();
    }
#endif
}
