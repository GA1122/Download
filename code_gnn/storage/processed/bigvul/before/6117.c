void ssl2_write_error(SSL *s)
{
    unsigned char buf[3];
    int i, error;

    buf[0] = SSL2_MT_ERROR;
    buf[1] = (s->error_code >> 8) & 0xff;
    buf[2] = (s->error_code) & 0xff;

 

    error = s->error;            
    s->error = 0;
    OPENSSL_assert(error >= 0 && error <= (int)sizeof(buf));
    i = ssl2_write(s, &(buf[3 - error]), error);

 

    if (i < 0)
        s->error = error;
    else {
        s->error = error - i;

        if (s->error == 0)
            if (s->msg_callback) {
                 
                s->msg_callback(1, s->version, 0, buf, 3, s,
                                s->msg_callback_arg);
            }
    }
}
