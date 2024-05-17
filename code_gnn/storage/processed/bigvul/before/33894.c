cib_remote_signoff(cib_t * cib)
{
    int rc = pcmk_ok;

     

    crm_debug("Signing out of the CIB Service");
#ifdef HAVE_GNUTLS_GNUTLS_H
    cib_tls_close(cib);
#endif

    cib->state = cib_disconnected;
    cib->type = cib_none;

    return rc;
}