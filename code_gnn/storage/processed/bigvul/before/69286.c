int ssl3_read_n(SSL *s, int n, int max, int extend, int clearold)
{
     
    int i, len, left;
    size_t align = 0;
    unsigned char *pkt;
    SSL3_BUFFER *rb;

    if (n <= 0)
        return n;

    rb = &s->rlayer.rbuf;
    if (rb->buf == NULL)
        if (!ssl3_setup_read_buffer(s))
            return -1;

    left = rb->left;
#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
    align = (size_t)rb->buf + SSL3_RT_HEADER_LENGTH;
    align = SSL3_ALIGN_PAYLOAD - 1 - ((align - 1) % SSL3_ALIGN_PAYLOAD);
#endif

    if (!extend) {
         
        if (left == 0)
            rb->offset = align;
        else if (align != 0 && left >= SSL3_RT_HEADER_LENGTH) {
             
            pkt = rb->buf + rb->offset;
            if (pkt[0] == SSL3_RT_APPLICATION_DATA
                && (pkt[3] << 8 | pkt[4]) >= 128) {
                 
                memmove(rb->buf + align, pkt, left);
                rb->offset = align;
            }
        }
        s->rlayer.packet = rb->buf + rb->offset;
        s->rlayer.packet_length = 0;
         
    }

    len = s->rlayer.packet_length;
    pkt = rb->buf + align;
     
    if (s->rlayer.packet != pkt && clearold == 1) {
        memmove(pkt, s->rlayer.packet, len + left);
        s->rlayer.packet = pkt;
        rb->offset = len + align;
    }

     
    if (SSL_IS_DTLS(s)) {
        if (left == 0 && extend)
            return 0;
        if (left > 0 && n > left)
            n = left;
    }

     
    if (left >= n) {
        s->rlayer.packet_length += n;
        rb->left = left - n;
        rb->offset += n;
        return (n);
    }

     

    if (n > (int)(rb->len - rb->offset)) {  
        SSLerr(SSL_F_SSL3_READ_N, ERR_R_INTERNAL_ERROR);
        return -1;
    }

     
    if (!s->rlayer.read_ahead && !SSL_IS_DTLS(s))
         
        max = n;
    else {
        if (max < n)
            max = n;
        if (max > (int)(rb->len - rb->offset))
            max = rb->len - rb->offset;
    }

    while (left < n) {
         

        clear_sys_error();
        if (s->rbio != NULL) {
            s->rwstate = SSL_READING;
            i = BIO_read(s->rbio, pkt + len + left, max - left);
        } else {
            SSLerr(SSL_F_SSL3_READ_N, SSL_R_READ_BIO_NOT_SET);
            i = -1;
        }

        if (i <= 0) {
            rb->left = left;
            if (s->mode & SSL_MODE_RELEASE_BUFFERS && !SSL_IS_DTLS(s))
                if (len + left == 0)
                    ssl3_release_read_buffer(s);
            return i;
        }
        left += i;
         
        if (SSL_IS_DTLS(s)) {
            if (n > left)
                n = left;        
        }
    }

     
    rb->offset += n;
    rb->left = left - n;
    s->rlayer.packet_length += n;
    s->rwstate = SSL_NOTHING;
    return (n);
}
