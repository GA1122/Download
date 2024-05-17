int do_dtls1_write(SSL *s, int type, const unsigned char *buf,
                   unsigned int len, int create_empty_fragment)
{
    unsigned char *p, *pseq;
    int i, mac_size, clear = 0;
    int prefix_len = 0;
    int eivlen;
    SSL3_RECORD wr;
    SSL3_BUFFER *wb;
    SSL_SESSION *sess;

    wb = &s->rlayer.wbuf[0];

     
    if (SSL3_BUFFER_get_left(wb) != 0) {
        OPENSSL_assert(0);       
        return (ssl3_write_pending(s, type, buf, len));
    }

     
    if (s->s3->alert_dispatch) {
        i = s->method->ssl_dispatch_alert(s);
        if (i <= 0)
            return (i);
         
    }

    if (len == 0 && !create_empty_fragment)
        return 0;

    sess = s->session;

    if ((sess == NULL) ||
        (s->enc_write_ctx == NULL) || (EVP_MD_CTX_md(s->write_hash) == NULL))
        clear = 1;

    if (clear)
        mac_size = 0;
    else {
        mac_size = EVP_MD_CTX_size(s->write_hash);
        if (mac_size < 0)
            goto err;
    }

    p = SSL3_BUFFER_get_buf(wb) + prefix_len;

     

    *(p++) = type & 0xff;
    SSL3_RECORD_set_type(&wr, type);
     
    if (s->method->version == DTLS_ANY_VERSION &&
        s->max_proto_version != DTLS1_BAD_VER) {
        *(p++) = DTLS1_VERSION >> 8;
        *(p++) = DTLS1_VERSION & 0xff;
    } else {
        *(p++) = s->version >> 8;
        *(p++) = s->version & 0xff;
    }

     
    pseq = p;
    p += 10;

     
    if (s->enc_write_ctx) {
        int mode = EVP_CIPHER_CTX_mode(s->enc_write_ctx);
        if (mode == EVP_CIPH_CBC_MODE) {
            eivlen = EVP_CIPHER_CTX_iv_length(s->enc_write_ctx);
            if (eivlen <= 1)
                eivlen = 0;
        }
         
        else if (mode == EVP_CIPH_GCM_MODE)
            eivlen = EVP_GCM_TLS_EXPLICIT_IV_LEN;
        else if (mode == EVP_CIPH_CCM_MODE)
            eivlen = EVP_CCM_TLS_EXPLICIT_IV_LEN;
        else
            eivlen = 0;
    } else
        eivlen = 0;

     
    SSL3_RECORD_set_data(&wr, p + eivlen);  
    SSL3_RECORD_set_length(&wr, (int)len);
    SSL3_RECORD_set_input(&wr, (unsigned char *)buf);

     

     
    if (s->compress != NULL) {
        if (!ssl3_do_compress(s, &wr)) {
            SSLerr(SSL_F_DO_DTLS1_WRITE, SSL_R_COMPRESSION_FAILURE);
            goto err;
        }
    } else {
        memcpy(SSL3_RECORD_get_data(&wr), SSL3_RECORD_get_input(&wr),
               SSL3_RECORD_get_length(&wr));
        SSL3_RECORD_reset_input(&wr);
    }

     

    if (mac_size != 0) {
        if (s->method->ssl3_enc->mac(s, &wr,
                                     &(p[SSL3_RECORD_get_length(&wr) + eivlen]),
                                     1) < 0)
            goto err;
        SSL3_RECORD_add_length(&wr, mac_size);
    }

     
    SSL3_RECORD_set_data(&wr, p);
    SSL3_RECORD_reset_input(&wr);

    if (eivlen)
        SSL3_RECORD_add_length(&wr, eivlen);

    if (s->method->ssl3_enc->enc(s, &wr, 1, 1) < 1)
        goto err;

     
     

     

    s2n(s->rlayer.d->w_epoch, pseq);

     
     

    memcpy(pseq, &(s->rlayer.write_sequence[2]), 6);
    pseq += 6;
    s2n(SSL3_RECORD_get_length(&wr), pseq);

    if (s->msg_callback)
        s->msg_callback(1, 0, SSL3_RT_HEADER, pseq - DTLS1_RT_HEADER_LENGTH,
                        DTLS1_RT_HEADER_LENGTH, s, s->msg_callback_arg);

     
    SSL3_RECORD_set_type(&wr, type);  
    SSL3_RECORD_add_length(&wr, DTLS1_RT_HEADER_LENGTH);

    ssl3_record_sequence_update(&(s->rlayer.write_sequence[0]));

    if (create_empty_fragment) {
         
        return wr.length;
    }

     
    SSL3_BUFFER_set_left(wb, prefix_len + SSL3_RECORD_get_length(&wr));
    SSL3_BUFFER_set_offset(wb, 0);

     
    s->rlayer.wpend_tot = len;
    s->rlayer.wpend_buf = buf;
    s->rlayer.wpend_type = type;
    s->rlayer.wpend_ret = len;

     
    return ssl3_write_pending(s, type, buf, len);
 err:
    return -1;
}
