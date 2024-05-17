hook_connect_gnutls_set_certificates (gnutls_session_t tls_session,
                                      const gnutls_datum_t *req_ca, int nreq,
                                      const gnutls_pk_algorithm_t *pk_algos,
                                      int pk_algos_len,
#if LIBGNUTLS_VERSION_NUMBER >= 0x020b00
                                      gnutls_retr2_st *answer)
#else
                                      gnutls_retr_st *answer)
#endif
{
    struct t_hook *ptr_hook;
    int rc;

    rc = -1;
    ptr_hook = weechat_hooks[HOOK_TYPE_CONNECT];
    while (ptr_hook)
    {
         
        if (!ptr_hook->deleted
            && HOOK_CONNECT(ptr_hook, gnutls_sess)
            && (*(HOOK_CONNECT(ptr_hook, gnutls_sess)) == tls_session))
        {
            rc = (int) (HOOK_CONNECT(ptr_hook, gnutls_cb))
                (ptr_hook->callback_data, tls_session, req_ca, nreq,
                 pk_algos, pk_algos_len, answer,
                 WEECHAT_HOOK_CONNECT_GNUTLS_CB_SET_CERT);
            break;
        }
        ptr_hook = ptr_hook->next_hook;
    }

    return rc;
}
