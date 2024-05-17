int ssl3_write(SSL *s, const void *buf, int len)
{
    int ret, n;

#if 0
    if (s->shutdown & SSL_SEND_SHUTDOWN) {
        s->rwstate = SSL_NOTHING;
        return (0);
    }
#endif
    clear_sys_error();
    if (s->s3->renegotiate)
        ssl3_renegotiate_check(s);

     
     
    if ((s->s3->flags & SSL3_FLAGS_POP_BUFFER) && (s->wbio == s->bbio)) {
         
        if (s->s3->delay_buf_pop_ret == 0) {
            ret = ssl3_write_bytes(s, SSL3_RT_APPLICATION_DATA, buf, len);
            if (ret <= 0)
                return (ret);

            s->s3->delay_buf_pop_ret = ret;
        }

        s->rwstate = SSL_WRITING;
        n = BIO_flush(s->wbio);
        if (n <= 0)
            return (n);
        s->rwstate = SSL_NOTHING;

         
        ssl_free_wbio_buffer(s);
        s->s3->flags &= ~SSL3_FLAGS_POP_BUFFER;

        ret = s->s3->delay_buf_pop_ret;
        s->s3->delay_buf_pop_ret = 0;
    } else {
        ret = s->method->ssl_write_bytes(s, SSL3_RT_APPLICATION_DATA,
                                         buf, len);
        if (ret <= 0)
            return (ret);
    }

    return (ret);
}
