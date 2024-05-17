int ssl3_shutdown(SSL *s)
{
    int ret;

     
    if ((s->quiet_shutdown) || (s->state == SSL_ST_BEFORE)) {
        s->shutdown = (SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
        return (1);
    }

    if (!(s->shutdown & SSL_SENT_SHUTDOWN)) {
        s->shutdown |= SSL_SENT_SHUTDOWN;
#if 1
        ssl3_send_alert(s, SSL3_AL_WARNING, SSL_AD_CLOSE_NOTIFY);
#endif
         
        if (s->s3->alert_dispatch)
            return (-1);         
    } else if (s->s3->alert_dispatch) {
         
#if 1
        ret = s->method->ssl_dispatch_alert(s);
        if (ret == -1) {
             
            return (ret);
        }
#endif
    } else if (!(s->shutdown & SSL_RECEIVED_SHUTDOWN)) {
        if (SSL_in_init(s)) {
             
            SSLerr(SSL_F_SSL3_SHUTDOWN, SSL_R_SHUTDOWN_WHILE_IN_INIT);
            return -1;
        }
         
        s->method->ssl_read_bytes(s, 0, NULL, 0, 0);
        if (!(s->shutdown & SSL_RECEIVED_SHUTDOWN)) {
            return (-1);         
        }
    }

    if ((s->shutdown == (SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN)) &&
        !s->s3->alert_dispatch)
        return (1);
    else
        return (0);
}
