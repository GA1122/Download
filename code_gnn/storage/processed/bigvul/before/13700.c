static int ssl3_read_internal(SSL *s, void *buf, int len, int peek)
{
    int ret;

    clear_sys_error();
    if (s->s3->renegotiate)
        ssl3_renegotiate_check(s);
    s->s3->in_read_app_data = 1;
    ret =
        s->method->ssl_read_bytes(s, SSL3_RT_APPLICATION_DATA, buf, len,
                                  peek);
    if ((ret == -1) && (s->s3->in_read_app_data == 2)) {
         
        s->in_handshake++;
        ret =
            s->method->ssl_read_bytes(s, SSL3_RT_APPLICATION_DATA, buf, len,
                                      peek);
        s->in_handshake--;
    } else
        s->s3->in_read_app_data = 0;

    return (ret);
}
