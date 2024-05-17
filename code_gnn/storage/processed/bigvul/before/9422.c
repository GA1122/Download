int ssl_check_clienthello_tlsext_late(SSL *s, int *al)
{
    s->tlsext_status_expected = 0;

     
    if ((s->tlsext_status_type != -1) && s->ctx && s->ctx->tlsext_status_cb) {
        int ret;
        CERT_PKEY *certpkey;
        certpkey = ssl_get_server_send_pkey(s);
         
        if (certpkey != NULL) {
             
            s->cert->key = certpkey;
            ret = s->ctx->tlsext_status_cb(s, s->ctx->tlsext_status_arg);
            switch (ret) {
                 
            case SSL_TLSEXT_ERR_NOACK:
                s->tlsext_status_expected = 0;
                break;
                 
            case SSL_TLSEXT_ERR_OK:
                if (s->tlsext_ocsp_resp)
                    s->tlsext_status_expected = 1;
                break;
                 
            case SSL_TLSEXT_ERR_ALERT_FATAL:
            default:
                *al = SSL_AD_INTERNAL_ERROR;
                return 0;
            }
        }
    }

    if (!tls1_alpn_handle_client_hello_late(s, al)) {
        return 0;
    }

    return 1;
}
