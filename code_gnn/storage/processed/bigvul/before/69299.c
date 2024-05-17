unsigned char *ssl_add_clienthello_tlsext(SSL *s, unsigned char *buf,
                                          unsigned char *limit, int *al)
{
    int extdatalen = 0;
    unsigned char *orig = buf;
    unsigned char *ret = buf;
#ifndef OPENSSL_NO_EC
     
    int using_ecc = 0;
    if (s->version >= TLS1_VERSION || SSL_IS_DTLS(s)) {
        int i;
        unsigned long alg_k, alg_a;
        STACK_OF(SSL_CIPHER) *cipher_stack = SSL_get_ciphers(s);

        for (i = 0; i < sk_SSL_CIPHER_num(cipher_stack); i++) {
            const SSL_CIPHER *c = sk_SSL_CIPHER_value(cipher_stack, i);

            alg_k = c->algorithm_mkey;
            alg_a = c->algorithm_auth;
            if ((alg_k & (SSL_kECDHE | SSL_kECDHEPSK))
                || (alg_a & SSL_aECDSA)) {
                using_ecc = 1;
                break;
            }
        }
    }
#endif

    ret += 2;

    if (ret >= limit)
        return NULL;             

     
    if (s->renegotiate) {
        int el;

        if (!ssl_add_clienthello_renegotiate_ext(s, 0, &el, 0)) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }

        if (CHECKLEN(ret, 4 + el, limit))
            return NULL;

        s2n(TLSEXT_TYPE_renegotiate, ret);
        s2n(el, ret);

        if (!ssl_add_clienthello_renegotiate_ext(s, ret, &el, el)) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }

        ret += el;
    }
     
    if (s->client_version == SSL3_VERSION)
        goto done;

    if (s->tlsext_hostname != NULL) {
         
        size_t size_str;

         
        size_str = strlen(s->tlsext_hostname);
        if (CHECKLEN(ret, 9 + size_str, limit))
            return NULL;

         
        s2n(TLSEXT_TYPE_server_name, ret);
        s2n(size_str + 5, ret);

         
        s2n(size_str + 3, ret);

         
        *(ret++) = (unsigned char)TLSEXT_NAMETYPE_host_name;
        s2n(size_str, ret);
        memcpy(ret, s->tlsext_hostname, size_str);
        ret += size_str;
    }
#ifndef OPENSSL_NO_SRP
     
    if (s->srp_ctx.login != NULL) {  

        size_t login_len = strlen(s->srp_ctx.login);
        if (login_len > 255 || login_len == 0) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }

         
        if (CHECKLEN(ret, 5 + login_len, limit))
            return NULL;

         
        s2n(TLSEXT_TYPE_srp, ret);
        s2n(login_len + 1, ret);
        (*ret++) = (unsigned char)login_len;
        memcpy(ret, s->srp_ctx.login, login_len);
        ret += login_len;
    }
#endif

#ifndef OPENSSL_NO_EC
    if (using_ecc) {
         
        const unsigned char *pcurves, *pformats;
        size_t num_curves, num_formats, curves_list_len;
        size_t i;
        unsigned char *etmp;

        tls1_get_formatlist(s, &pformats, &num_formats);

        if (num_formats > 255) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }
         
        if (CHECKLEN(ret, 5 + num_formats, limit))
            return NULL;

        s2n(TLSEXT_TYPE_ec_point_formats, ret);
         
        s2n(num_formats + 1, ret);
        *(ret++) = (unsigned char)num_formats;
        memcpy(ret, pformats, num_formats);
        ret += num_formats;

         
        pcurves = s->tlsext_ellipticcurvelist;
        if (!tls1_get_curvelist(s, 0, &pcurves, &num_curves))
            return NULL;

        if (num_curves > 65532 / 2) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }
         
        if (CHECKLEN(ret, 6 + (num_curves * 2), limit))
            return NULL;

        s2n(TLSEXT_TYPE_elliptic_curves, ret);
        etmp = ret + 4;
         
        for (i = 0; i < num_curves; i++, pcurves += 2) {
            if (tls_curve_allowed(s, pcurves, SSL_SECOP_CURVE_SUPPORTED)) {
                *etmp++ = pcurves[0];
                *etmp++ = pcurves[1];
            }
        }

        curves_list_len = etmp - ret - 4;

        s2n(curves_list_len + 2, ret);
        s2n(curves_list_len, ret);
        ret += curves_list_len;
    }
#endif                           

    if (tls_use_ticket(s)) {
        size_t ticklen;
        if (!s->new_session && s->session && s->session->tlsext_tick)
            ticklen = s->session->tlsext_ticklen;
        else if (s->session && s->tlsext_session_ticket &&
                 s->tlsext_session_ticket->data) {
            ticklen = s->tlsext_session_ticket->length;
            s->session->tlsext_tick = OPENSSL_malloc(ticklen);
            if (s->session->tlsext_tick == NULL)
                return NULL;
            memcpy(s->session->tlsext_tick,
                   s->tlsext_session_ticket->data, ticklen);
            s->session->tlsext_ticklen = ticklen;
        } else
            ticklen = 0;
        if (ticklen == 0 && s->tlsext_session_ticket &&
            s->tlsext_session_ticket->data == NULL)
            goto skip_ext;
         
        if (CHECKLEN(ret, 4 + ticklen, limit))
            return NULL;
        s2n(TLSEXT_TYPE_session_ticket, ret);
        s2n(ticklen, ret);
        if (ticklen > 0) {
            memcpy(ret, s->session->tlsext_tick, ticklen);
            ret += ticklen;
        }
    }
 skip_ext:

    if (SSL_CLIENT_USE_SIGALGS(s)) {
        size_t salglen;
        const unsigned char *salg;
        unsigned char *etmp;
        salglen = tls12_get_psigalgs(s, 1, &salg);

         
        if (CHECKLEN(ret, salglen + 6, limit))
            return NULL;
        s2n(TLSEXT_TYPE_signature_algorithms, ret);
        etmp = ret;
         
        ret += 4;
        salglen = tls12_copy_sigalgs(s, ret, salg, salglen);
         
        s2n(salglen + 2, etmp);
        s2n(salglen, etmp);
        ret += salglen;
    }
#ifndef OPENSSL_NO_OCSP
    if (s->tlsext_status_type == TLSEXT_STATUSTYPE_ocsp) {
        int i;
        size_t extlen, idlen;
        int lentmp;
        OCSP_RESPID *id;

        idlen = 0;
        for (i = 0; i < sk_OCSP_RESPID_num(s->tlsext_ocsp_ids); i++) {
            id = sk_OCSP_RESPID_value(s->tlsext_ocsp_ids, i);
            lentmp = i2d_OCSP_RESPID(id, NULL);
            if (lentmp <= 0)
                return NULL;
            idlen += (size_t)lentmp + 2;
        }

        if (s->tlsext_ocsp_exts) {
            lentmp = i2d_X509_EXTENSIONS(s->tlsext_ocsp_exts, NULL);
            if (lentmp < 0)
                return NULL;
            extlen = (size_t)lentmp;
        } else
            extlen = 0;

        if (extlen + idlen > 0xFFF0)
            return NULL;
         
        if (CHECKLEN(ret, 9 + idlen + extlen, limit))
            return NULL;

        s2n(TLSEXT_TYPE_status_request, ret);
        s2n(extlen + idlen + 5, ret);
        *(ret++) = TLSEXT_STATUSTYPE_ocsp;
        s2n(idlen, ret);
        for (i = 0; i < sk_OCSP_RESPID_num(s->tlsext_ocsp_ids); i++) {
             
            unsigned char *q = ret;
            id = sk_OCSP_RESPID_value(s->tlsext_ocsp_ids, i);
             
            ret += 2;
            lentmp = i2d_OCSP_RESPID(id, &ret);
             
            s2n(lentmp, q);
        }
        s2n(extlen, ret);
        if (extlen > 0)
            i2d_X509_EXTENSIONS(s->tlsext_ocsp_exts, &ret);
    }
#endif
#ifndef OPENSSL_NO_HEARTBEATS
    if (SSL_IS_DTLS(s)) {
         

         
        if (CHECKLEN(ret, 5, limit))
            return NULL;

        s2n(TLSEXT_TYPE_heartbeat, ret);
        s2n(1, ret);
         
        if (s->tlsext_heartbeat & SSL_DTLSEXT_HB_DONT_RECV_REQUESTS)
            *(ret++) = SSL_DTLSEXT_HB_DONT_SEND_REQUESTS;
        else
            *(ret++) = SSL_DTLSEXT_HB_ENABLED;
    }
#endif

#ifndef OPENSSL_NO_NEXTPROTONEG
    if (s->ctx->next_proto_select_cb && !s->s3->tmp.finish_md_len) {
         

         
        if (CHECKLEN(ret, 4, limit))
            return NULL;
        s2n(TLSEXT_TYPE_next_proto_neg, ret);
        s2n(0, ret);
    }
#endif

     
    if (s->alpn_client_proto_list && !s->s3->tmp.finish_md_len) {
         
        if (CHECKLEN(ret, 6 + s->alpn_client_proto_list_len, limit))
            return NULL;
        s2n(TLSEXT_TYPE_application_layer_protocol_negotiation, ret);
        s2n(2 + s->alpn_client_proto_list_len, ret);
        s2n(s->alpn_client_proto_list_len, ret);
        memcpy(ret, s->alpn_client_proto_list, s->alpn_client_proto_list_len);
        ret += s->alpn_client_proto_list_len;
        s->s3->alpn_sent = 1;
    }
#ifndef OPENSSL_NO_SRTP
    if (SSL_IS_DTLS(s) && SSL_get_srtp_profiles(s)) {
        int el;

         
        if (ssl_add_clienthello_use_srtp_ext(s, 0, &el, 0)) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }

         
        if (CHECKLEN(ret, 4 + el, limit))
            return NULL;

        s2n(TLSEXT_TYPE_use_srtp, ret);
        s2n(el, ret);

        if (ssl_add_clienthello_use_srtp_ext(s, ret, &el, el)) {
            SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
            return NULL;
        }
        ret += el;
    }
#endif
    custom_ext_init(&s->cert->cli_ext);
     
    if (!custom_ext_add(s, 0, &ret, limit, al))
        return NULL;
     
    if (!SSL_IS_DTLS(s)) {
         
        if (CHECKLEN(ret, 4, limit))
            return NULL;
        s2n(TLSEXT_TYPE_encrypt_then_mac, ret);
        s2n(0, ret);
    }

#ifndef OPENSSL_NO_CT
    if (s->ct_validation_callback != NULL) {
         
        if (CHECKLEN(ret, 4, limit))
            return NULL;

        s2n(TLSEXT_TYPE_signed_certificate_timestamp, ret);
        s2n(0, ret);
    }
#endif

     
    if (CHECKLEN(ret, 4, limit))
        return NULL;
    s2n(TLSEXT_TYPE_extended_master_secret, ret);
    s2n(0, ret);

     
    if (s->options & SSL_OP_TLSEXT_PADDING) {
        int hlen = ret - (unsigned char *)s->init_buf->data;

        if (hlen > 0xff && hlen < 0x200) {
            hlen = 0x200 - hlen;
            if (hlen >= 4)
                hlen -= 4;
            else
                hlen = 0;

             
            if (CHECKLEN(ret, 4 + hlen, limit))
                return NULL;
            s2n(TLSEXT_TYPE_padding, ret);
            s2n(hlen, ret);
            memset(ret, 0, hlen);
            ret += hlen;
        }
    }

 done:

    if ((extdatalen = ret - orig - 2) == 0)
        return orig;

    s2n(extdatalen, orig);
    return ret;
}
