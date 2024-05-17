int ssl3_renegotiate_check(SSL *s)
{
    int ret = 0;

    if (s->s3->renegotiate) {
        if ((s->s3->rbuf.left == 0) &&
            (s->s3->wbuf.left == 0) && !SSL_in_init(s)) {
             
             
            s->state = SSL_ST_RENEGOTIATE;
            s->s3->renegotiate = 0;
            s->s3->num_renegotiations++;
            s->s3->total_renegotiations++;
            ret = 1;
        }
    }
    return (ret);
}
