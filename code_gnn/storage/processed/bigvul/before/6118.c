static int get_client_finished(SSL *s)
{
    unsigned char *p;
    int i, n;
    unsigned long len;

    p = (unsigned char *)s->init_buf->data;
    if (s->state == SSL2_ST_GET_CLIENT_FINISHED_A) {
        i = ssl2_read(s, (char *)&(p[s->init_num]), 1 - s->init_num);
        if (i < 1 - s->init_num)
            return (ssl2_part_read(s, SSL_F_GET_CLIENT_FINISHED, i));
        s->init_num += i;

        if (*p != SSL2_MT_CLIENT_FINISHED) {
            if (*p != SSL2_MT_ERROR) {
                ssl2_return_error(s, SSL2_PE_UNDEFINED_ERROR);
                SSLerr(SSL_F_GET_CLIENT_FINISHED,
                       SSL_R_READ_WRONG_PACKET_TYPE);
            } else {
                SSLerr(SSL_F_GET_CLIENT_FINISHED, SSL_R_PEER_ERROR);
                 
                i = ssl2_read(s, (char *)&(p[s->init_num]), 3 - s->init_num);
                return ssl2_part_read(s, SSL_F_GET_SERVER_VERIFY, i);
            }
            return (-1);
        }
        s->state = SSL2_ST_GET_CLIENT_FINISHED_B;
    }

     
    if (s->s2->conn_id_length > sizeof s->s2->conn_id) {
        ssl2_return_error(s, SSL2_PE_UNDEFINED_ERROR);
        SSLerr(SSL_F_GET_CLIENT_FINISHED, ERR_R_INTERNAL_ERROR);
        return -1;
    }
    len = 1 + (unsigned long)s->s2->conn_id_length;
    n = (int)len - s->init_num;
    i = ssl2_read(s, (char *)&(p[s->init_num]), n);
    if (i < n) {
        return (ssl2_part_read(s, SSL_F_GET_CLIENT_FINISHED, i));
    }
    if (s->msg_callback) {
         
        s->msg_callback(0, s->version, 0, p, len, s, s->msg_callback_arg);
    }
    p += 1;
    if (memcmp(p, s->s2->conn_id, s->s2->conn_id_length) != 0) {
        ssl2_return_error(s, SSL2_PE_UNDEFINED_ERROR);
        SSLerr(SSL_F_GET_CLIENT_FINISHED, SSL_R_CONNECTION_ID_IS_DIFFERENT);
        return (-1);
    }
    return (1);
}
