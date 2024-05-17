int do_ssl3_write(SSL *s, int type, const unsigned char *buf,
                  unsigned int *pipelens, unsigned int numpipes,
                  int create_empty_fragment)
{
    unsigned char *outbuf[SSL_MAX_PIPELINES], *plen[SSL_MAX_PIPELINES];
    SSL3_RECORD wr[SSL_MAX_PIPELINES];
    int i, mac_size, clear = 0;
    int prefix_len = 0;
    int eivlen;
    size_t align = 0;
    SSL3_BUFFER *wb;
    SSL_SESSION *sess;
    unsigned int totlen = 0;
    unsigned int j;

    for (j = 0; j < numpipes; j++)
        totlen += pipelens[j];
     
    if (RECORD_LAYER_write_pending(&s->rlayer))
        return (ssl3_write_pending(s, type, buf, totlen));

     
    if (s->s3->alert_dispatch) {
        i = s->method->ssl_dispatch_alert(s);
        if (i <= 0)
            return (i);
         
    }

    if (s->rlayer.numwpipes < numpipes)
        if (!ssl3_setup_write_buffer(s, numpipes, 0))
            return -1;

    if (totlen == 0 && !create_empty_fragment)
        return 0;

    sess = s->session;

    if ((sess == NULL) ||
        (s->enc_write_ctx == NULL) || (EVP_MD_CTX_md(s->write_hash) == NULL)) {
        clear = s->enc_write_ctx ? 0 : 1;  
        mac_size = 0;
    } else {
        mac_size = EVP_MD_CTX_size(s->write_hash);
        if (mac_size < 0)
            goto err;
    }

     
    if (!clear && !create_empty_fragment && !s->s3->empty_fragment_done) {
         

        if (s->s3->need_empty_fragments && type == SSL3_RT_APPLICATION_DATA) {
             
            unsigned int tmppipelen = 0;

            prefix_len = do_ssl3_write(s, type, buf, &tmppipelen, 1, 1);
            if (prefix_len <= 0)
                goto err;

            if (prefix_len >
                (SSL3_RT_HEADER_LENGTH + SSL3_RT_SEND_MAX_ENCRYPTED_OVERHEAD)) {
                 
                SSLerr(SSL_F_DO_SSL3_WRITE, ERR_R_INTERNAL_ERROR);
                goto err;
            }
        }

        s->s3->empty_fragment_done = 1;
    }

    if (create_empty_fragment) {
        wb = &s->rlayer.wbuf[0];
#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
         
        align = (size_t)SSL3_BUFFER_get_buf(wb) + 2 * SSL3_RT_HEADER_LENGTH;
        align = SSL3_ALIGN_PAYLOAD - 1 - ((align - 1) % SSL3_ALIGN_PAYLOAD);
#endif
        outbuf[0] = SSL3_BUFFER_get_buf(wb) + align;
        SSL3_BUFFER_set_offset(wb, align);
    } else if (prefix_len) {
        wb = &s->rlayer.wbuf[0];
        outbuf[0] = SSL3_BUFFER_get_buf(wb) + SSL3_BUFFER_get_offset(wb)
            + prefix_len;
    } else {
        for (j = 0; j < numpipes; j++) {
            wb = &s->rlayer.wbuf[j];
#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
            align = (size_t)SSL3_BUFFER_get_buf(wb) + SSL3_RT_HEADER_LENGTH;
            align = SSL3_ALIGN_PAYLOAD - 1 - ((align - 1) % SSL3_ALIGN_PAYLOAD);
#endif
            outbuf[j] = SSL3_BUFFER_get_buf(wb) + align;
            SSL3_BUFFER_set_offset(wb, align);
        }
    }

     
    if (s->enc_write_ctx && SSL_USE_EXPLICIT_IV(s)) {
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

    totlen = 0;
     
    memset(wr, 0, sizeof wr);
    for (j = 0; j < numpipes; j++) {
         
        *(outbuf[j]++) = type & 0xff;
        SSL3_RECORD_set_type(&wr[j], type);

        *(outbuf[j]++) = (s->version >> 8);
         
        if (SSL_get_state(s) == TLS_ST_CW_CLNT_HELLO
            && !s->renegotiate && TLS1_get_version(s) > TLS1_VERSION)
            *(outbuf[j]++) = 0x1;
        else
            *(outbuf[j]++) = s->version & 0xff;

         
        plen[j] = outbuf[j];
        outbuf[j] += 2;

         
        SSL3_RECORD_set_data(&wr[j], outbuf[j] + eivlen);
        SSL3_RECORD_set_length(&wr[j], (int)pipelens[j]);
        SSL3_RECORD_set_input(&wr[j], (unsigned char *)&buf[totlen]);
        totlen += pipelens[j];

         

         
        if (s->compress != NULL) {
            if (!ssl3_do_compress(s, &wr[j])) {
                SSLerr(SSL_F_DO_SSL3_WRITE, SSL_R_COMPRESSION_FAILURE);
                goto err;
            }
        } else {
            memcpy(wr[j].data, wr[j].input, wr[j].length);
            SSL3_RECORD_reset_input(&wr[j]);
        }

         

        if (!SSL_USE_ETM(s) && mac_size != 0) {
            if (s->method->ssl3_enc->mac(s, &wr[j],
                                         &(outbuf[j][wr[j].length + eivlen]),
                                         1) < 0)
                goto err;
            SSL3_RECORD_add_length(&wr[j], mac_size);
        }

        SSL3_RECORD_set_data(&wr[j], outbuf[j]);
        SSL3_RECORD_reset_input(&wr[j]);

        if (eivlen) {
             
            SSL3_RECORD_add_length(&wr[j], eivlen);
        }
    }

    if (s->method->ssl3_enc->enc(s, wr, numpipes, 1) < 1)
        goto err;

    for (j = 0; j < numpipes; j++) {
        if (SSL_USE_ETM(s) && mac_size != 0) {
            if (s->method->ssl3_enc->mac(s, &wr[j],
                                         outbuf[j] + wr[j].length, 1) < 0)
                goto err;
            SSL3_RECORD_add_length(&wr[j], mac_size);
        }

         
        s2n(SSL3_RECORD_get_length(&wr[j]), plen[j]);

        if (s->msg_callback)
            s->msg_callback(1, 0, SSL3_RT_HEADER, plen[j] - 5, 5, s,
                            s->msg_callback_arg);

         
        SSL3_RECORD_set_type(&wr[j], type);  
        SSL3_RECORD_add_length(&wr[j], SSL3_RT_HEADER_LENGTH);

        if (create_empty_fragment) {
             
            if (j > 0) {
                 
                SSLerr(SSL_F_DO_SSL3_WRITE, ERR_R_INTERNAL_ERROR);
                goto err;
            }
            return SSL3_RECORD_get_length(wr);
        }

         
        SSL3_BUFFER_set_left(&s->rlayer.wbuf[j],
                             prefix_len + SSL3_RECORD_get_length(&wr[j]));
    }

     
    s->rlayer.wpend_tot = totlen;
    s->rlayer.wpend_buf = buf;
    s->rlayer.wpend_type = type;
    s->rlayer.wpend_ret = totlen;

     
    return ssl3_write_pending(s, type, buf, totlen);
 err:
    return -1;
}
