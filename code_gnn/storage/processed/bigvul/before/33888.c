cib_remote_connection_destroy(gpointer user_data)
{
    crm_err("Connection destroyed");
#ifdef HAVE_GNUTLS_GNUTLS_H
    cib_tls_close(user_data);
#endif
    return;
 }