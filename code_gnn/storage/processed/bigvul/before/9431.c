static int ssl_scan_clienthello_tlsext(SSL *s, PACKET *pkt, int *al)
{
    unsigned int type;
    int renegotiate_seen = 0;
    PACKET extensions;

    *al = SSL_AD_DECODE_ERROR;
    s->servername_done = 0;
    s->tlsext_status_type = -1;
#ifndef OPENSSL_NO_NEXTPROTONEG
    s->s3->next_proto_neg_seen = 0;
#endif

    OPENSSL_free(s->s3->alpn_selected);
    s->s3->alpn_selected = NULL;
    s->s3->alpn_selected_len = 0;
    OPENSSL_free(s->s3->alpn_proposed);
    s->s3->alpn_proposed = NULL;
    s->s3->alpn_proposed_len = 0;
#ifndef OPENSSL_NO_HEARTBEATS
    s->tlsext_heartbeat &= ~(SSL_DTLSEXT_HB_ENABLED |
                             SSL_DTLSEXT_HB_DONT_SEND_REQUESTS);
#endif

#ifndef OPENSSL_NO_EC
    if (s->options & SSL_OP_SAFARI_ECDHE_ECDSA_BUG)
        ssl_check_for_safari(s, pkt);
#endif                           

     
    OPENSSL_free(s->s3->tmp.peer_sigalgs);
    s->s3->tmp.peer_sigalgs = NULL;
    s->s3->flags &= ~TLS1_FLAGS_ENCRYPT_THEN_MAC;

#ifndef OPENSSL_NO_SRP
    OPENSSL_free(s->srp_ctx.login);
    s->srp_ctx.login = NULL;
#endif

    s->srtp_profile = NULL;

    if (PACKET_remaining(pkt) == 0)
        goto ri_check;

    if (!PACKET_as_length_prefixed_2(pkt, &extensions))
        return 0;

    if (!tls1_check_duplicate_extensions(&extensions))
        return 0;

     
    while (PACKET_get_net_2(&extensions, &type)) {
        PACKET extension;
        if (!PACKET_get_length_prefixed_2(&extensions, &extension))
            return 0;

        if (s->tlsext_debug_cb)
            s->tlsext_debug_cb(s, 0, type, PACKET_data(&extension),
                               PACKET_remaining(&extension),
                               s->tlsext_debug_arg);

        if (type == TLSEXT_TYPE_renegotiate) {
            if (!ssl_parse_clienthello_renegotiate_ext(s, &extension, al))
                return 0;
            renegotiate_seen = 1;
        } else if (s->version == SSL3_VERSION) {
        }
 

        else if (type == TLSEXT_TYPE_server_name) {
            unsigned int servname_type;
            PACKET sni, hostname;

            if (!PACKET_as_length_prefixed_2(&extension, &sni)
                 
                || PACKET_remaining(&sni) == 0) {
                return 0;
            }

             
            if (!PACKET_get_1(&sni, &servname_type)
                || servname_type != TLSEXT_NAMETYPE_host_name
                || !PACKET_as_length_prefixed_2(&sni, &hostname)) {
                return 0;
            }

            if (!s->hit) {
                if (PACKET_remaining(&hostname) > TLSEXT_MAXLEN_host_name) {
                    *al = TLS1_AD_UNRECOGNIZED_NAME;
                    return 0;
                }

                if (PACKET_contains_zero_byte(&hostname)) {
                    *al = TLS1_AD_UNRECOGNIZED_NAME;
                    return 0;
                }

                if (!PACKET_strndup(&hostname, &s->session->tlsext_hostname)) {
                    *al = TLS1_AD_INTERNAL_ERROR;
                    return 0;
                }

                s->servername_done = 1;
            } else {
                 
                s->servername_done = s->session->tlsext_hostname
                    && PACKET_equal(&hostname, s->session->tlsext_hostname,
                                    strlen(s->session->tlsext_hostname));
            }
        }
#ifndef OPENSSL_NO_SRP
        else if (type == TLSEXT_TYPE_srp) {
            PACKET srp_I;

            if (!PACKET_as_length_prefixed_1(&extension, &srp_I))
                return 0;

            if (PACKET_contains_zero_byte(&srp_I))
                return 0;

             
            if (!PACKET_strndup(&srp_I, &s->srp_ctx.login)) {
                *al = TLS1_AD_INTERNAL_ERROR;
                return 0;
            }
        }
#endif

#ifndef OPENSSL_NO_EC
        else if (type == TLSEXT_TYPE_ec_point_formats) {
            PACKET ec_point_format_list;

            if (!PACKET_as_length_prefixed_1(&extension, &ec_point_format_list)
                || PACKET_remaining(&ec_point_format_list) == 0) {
                return 0;
            }

            if (!s->hit) {
                if (!PACKET_memdup(&ec_point_format_list,
                                   &s->session->tlsext_ecpointformatlist,
                                   &s->
                                   session->tlsext_ecpointformatlist_length)) {
                    *al = TLS1_AD_INTERNAL_ERROR;
                    return 0;
                }
            }
        } else if (type == TLSEXT_TYPE_elliptic_curves) {
            PACKET elliptic_curve_list;

             
            if (!PACKET_as_length_prefixed_2(&extension, &elliptic_curve_list)
                || PACKET_remaining(&elliptic_curve_list) == 0
                || (PACKET_remaining(&elliptic_curve_list) % 2) != 0) {
                return 0;
            }

            if (!s->hit) {
                if (!PACKET_memdup(&elliptic_curve_list,
                                   &s->session->tlsext_ellipticcurvelist,
                                   &s->
                                   session->tlsext_ellipticcurvelist_length)) {
                    *al = TLS1_AD_INTERNAL_ERROR;
                    return 0;
                }
            }
        }
#endif                           
        else if (type == TLSEXT_TYPE_session_ticket) {
            if (s->tls_session_ticket_ext_cb &&
                !s->tls_session_ticket_ext_cb(s, PACKET_data(&extension),
                                              PACKET_remaining(&extension),
                                              s->tls_session_ticket_ext_cb_arg))
            {
                *al = TLS1_AD_INTERNAL_ERROR;
                return 0;
            }
        } else if (type == TLSEXT_TYPE_signature_algorithms) {
            PACKET supported_sig_algs;

            if (!PACKET_as_length_prefixed_2(&extension, &supported_sig_algs)
                || (PACKET_remaining(&supported_sig_algs) % 2) != 0
                || PACKET_remaining(&supported_sig_algs) == 0) {
                return 0;
            }

            if (!s->hit) {
                if (!tls1_save_sigalgs(s, PACKET_data(&supported_sig_algs),
                                       PACKET_remaining(&supported_sig_algs))) {
                    return 0;
                }
            }
        } else if (type == TLSEXT_TYPE_status_request) {
            if (!PACKET_get_1(&extension,
                              (unsigned int *)&s->tlsext_status_type)) {
                return 0;
            }
#ifndef OPENSSL_NO_OCSP
            if (s->tlsext_status_type == TLSEXT_STATUSTYPE_ocsp) {
                const unsigned char *ext_data;
                PACKET responder_id_list, exts;
                if (!PACKET_get_length_prefixed_2
                    (&extension, &responder_id_list))
                    return 0;

                while (PACKET_remaining(&responder_id_list) > 0) {
                    OCSP_RESPID *id;
                    PACKET responder_id;
                    const unsigned char *id_data;

                    if (!PACKET_get_length_prefixed_2(&responder_id_list,
                                                      &responder_id)
                        || PACKET_remaining(&responder_id) == 0) {
                        return 0;
                    }

                    if (s->tlsext_ocsp_ids == NULL
                        && (s->tlsext_ocsp_ids =
                            sk_OCSP_RESPID_new_null()) == NULL) {
                        *al = SSL_AD_INTERNAL_ERROR;
                        return 0;
                    }

                    id_data = PACKET_data(&responder_id);
                    id = d2i_OCSP_RESPID(NULL, &id_data,
                                         PACKET_remaining(&responder_id));
                    if (id == NULL)
                        return 0;

                    if (id_data != PACKET_end(&responder_id)) {
                        OCSP_RESPID_free(id);
                        return 0;
                    }

                    if (!sk_OCSP_RESPID_push(s->tlsext_ocsp_ids, id)) {
                        OCSP_RESPID_free(id);
                        *al = SSL_AD_INTERNAL_ERROR;
                        return 0;
                    }
                }

                 
                if (!PACKET_as_length_prefixed_2(&extension, &exts))
                    return 0;

                if (PACKET_remaining(&exts) > 0) {
                    ext_data = PACKET_data(&exts);
                    sk_X509_EXTENSION_pop_free(s->tlsext_ocsp_exts,
                                               X509_EXTENSION_free);
                    s->tlsext_ocsp_exts =
                        d2i_X509_EXTENSIONS(NULL, &ext_data,
                                            PACKET_remaining(&exts));
                    if (s->tlsext_ocsp_exts == NULL
                        || ext_data != PACKET_end(&exts)) {
                        return 0;
                    }
                }
            } else
#endif
            {
                 
                s->tlsext_status_type = -1;
            }
        }
#ifndef OPENSSL_NO_HEARTBEATS
        else if (SSL_IS_DTLS(s) && type == TLSEXT_TYPE_heartbeat) {
            unsigned int hbtype;

            if (!PACKET_get_1(&extension, &hbtype)
                || PACKET_remaining(&extension)) {
                *al = SSL_AD_DECODE_ERROR;
                return 0;
            }
            switch (hbtype) {
            case 0x01:          
                s->tlsext_heartbeat |= SSL_DTLSEXT_HB_ENABLED;
                break;
            case 0x02:          
                s->tlsext_heartbeat |= SSL_DTLSEXT_HB_ENABLED;
                s->tlsext_heartbeat |= SSL_DTLSEXT_HB_DONT_SEND_REQUESTS;
                break;
            default:
                *al = SSL_AD_ILLEGAL_PARAMETER;
                return 0;
            }
        }
#endif
#ifndef OPENSSL_NO_NEXTPROTONEG
        else if (type == TLSEXT_TYPE_next_proto_neg &&
                 s->s3->tmp.finish_md_len == 0) {
             
            s->s3->next_proto_neg_seen = 1;
        }
#endif

        else if (type == TLSEXT_TYPE_application_layer_protocol_negotiation &&
                 s->s3->tmp.finish_md_len == 0) {
            if (!tls1_alpn_handle_client_hello(s, &extension, al))
                return 0;
        }

         
#ifndef OPENSSL_NO_SRTP
        else if (SSL_IS_DTLS(s) && SSL_get_srtp_profiles(s)
                 && type == TLSEXT_TYPE_use_srtp) {
            if (ssl_parse_clienthello_use_srtp_ext(s, &extension, al))
                return 0;
        }
#endif
        else if (type == TLSEXT_TYPE_encrypt_then_mac)
            s->s3->flags |= TLS1_FLAGS_ENCRYPT_THEN_MAC;
         

         
        else if (!s->hit) {
            if (custom_ext_parse(s, 1, type, PACKET_data(&extension),
                                 PACKET_remaining(&extension), al) <= 0)
                return 0;
        }
    }

    if (PACKET_remaining(pkt) != 0) {
         
        *al = SSL_AD_INTERNAL_ERROR;
        return 0;
    }

 ri_check:

     

    if (!renegotiate_seen && s->renegotiate &&
        !(s->options & SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION)) {
        *al = SSL_AD_HANDSHAKE_FAILURE;
        SSLerr(SSL_F_SSL_SCAN_CLIENTHELLO_TLSEXT,
               SSL_R_UNSAFE_LEGACY_RENEGOTIATION_DISABLED);
        return 0;
    }

     
    return 1;
}
