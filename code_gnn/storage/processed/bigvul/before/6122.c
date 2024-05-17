static int server_verify(SSL *s)
{
    unsigned char *p;

    if (s->state == SSL2_ST_SEND_SERVER_VERIFY_A) {
        p = (unsigned char *)s->init_buf->data;
        *(p++) = SSL2_MT_SERVER_VERIFY;
        if (s->s2->challenge_length > sizeof s->s2->challenge) {
            SSLerr(SSL_F_SERVER_VERIFY, ERR_R_INTERNAL_ERROR);
            return -1;
        }
        memcpy(p, s->s2->challenge, (unsigned int)s->s2->challenge_length);
         

        s->state = SSL2_ST_SEND_SERVER_VERIFY_B;
        s->init_num = s->s2->challenge_length + 1;
        s->init_off = 0;
    }
    return (ssl2_do_write(s));
}
