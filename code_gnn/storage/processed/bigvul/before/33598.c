process_tgs_req(struct server_handle *handle, krb5_data *pkt,
                const krb5_fulladdr *from, krb5_data **response)
{
    krb5_keyblock * subkey = 0;
    krb5_keyblock * tgskey = 0;
    krb5_kdc_req *request = 0;
    krb5_db_entry *server = NULL;
    krb5_db_entry *stkt_server = NULL;
    krb5_kdc_rep reply;
    krb5_enc_kdc_rep_part reply_encpart;
    krb5_ticket ticket_reply, *header_ticket = 0;
    int st_idx = 0;
    krb5_enc_tkt_part enc_tkt_reply;
    krb5_transited enc_tkt_transited;
    int newtransited = 0;
    krb5_error_code retval = 0;
    krb5_keyblock encrypting_key;
    krb5_timestamp kdc_time, authtime = 0;
    krb5_keyblock session_key;
    krb5_timestamp rtime;
    krb5_keyblock *reply_key = NULL;
    krb5_key_data  *server_key;
    krb5_principal cprinc = NULL, sprinc = NULL, altcprinc = NULL;
    krb5_last_req_entry *nolrarray[2], nolrentry;
    int errcode;
    const char        *status = 0;
    krb5_enc_tkt_part *header_enc_tkt = NULL;  
    krb5_enc_tkt_part *subject_tkt = NULL;  
    krb5_db_entry *client = NULL, *krbtgt = NULL;
    krb5_pa_s4u_x509_user *s4u_x509_user = NULL;  
    krb5_authdata **kdc_issued_auth_data = NULL;  
    unsigned int c_flags = 0, s_flags = 0;        
    krb5_boolean is_referral;
    const char *emsg = NULL;
    krb5_kvno ticket_kvno = 0;
    struct kdc_request_state *state = NULL;
    krb5_pa_data *pa_tgs_req;  
    krb5_data scratch;
    krb5_pa_data **e_data = NULL;
    kdc_realm_t *kdc_active_realm = NULL;

    reply.padata = 0;  
    reply_encpart.enc_padata = 0;
    enc_tkt_reply.authorization_data = NULL;

    session_key.contents = NULL;

    retval = decode_krb5_tgs_req(pkt, &request);
    if (retval)
        return retval;
    if (request->msg_type != KRB5_TGS_REQ) {
        krb5_free_kdc_req(handle->kdc_err_context, request);
        return KRB5_BADMSGTYPE;
    }

     
    kdc_active_realm = setup_server_realm(handle, request->server);
    if (kdc_active_realm == NULL) {
        krb5_free_kdc_req(handle->kdc_err_context, request);
        return KRB5KDC_ERR_WRONG_REALM;
    }
    errcode = kdc_make_rstate(kdc_active_realm, &state);
    if (errcode !=0) {
        krb5_free_kdc_req(handle->kdc_err_context, request);
        return errcode;
    }
    errcode = kdc_process_tgs_req(kdc_active_realm,
                                  request, from, pkt, &header_ticket,
                                  &krbtgt, &tgskey, &subkey, &pa_tgs_req);
    if (header_ticket && header_ticket->enc_part2)
        cprinc = header_ticket->enc_part2->client;

    if (errcode) {
        status = "PROCESS_TGS";
        goto cleanup;
    }

    if (!header_ticket) {
        errcode = KRB5_NO_TKT_SUPPLIED;         
        status="UNEXPECTED NULL in header_ticket";
        goto cleanup;
    }
    scratch.length = pa_tgs_req->length;
    scratch.data = (char *) pa_tgs_req->contents;
    errcode = kdc_find_fast(&request, &scratch, subkey,
                            header_ticket->enc_part2->session, state, NULL);
    if (errcode !=0) {
        status = "kdc_find_fast";
        goto cleanup;
    }

     
    header_enc_tkt = header_ticket->enc_part2;

     

     

    setflag(s_flags, KRB5_KDB_FLAG_ALIAS_OK);
    if (isflagset(request->kdc_options, KDC_OPT_CANONICALIZE)) {
        setflag(c_flags, KRB5_KDB_FLAG_CANONICALIZE);
        setflag(s_flags, KRB5_KDB_FLAG_CANONICALIZE);
    }

    errcode = search_sprinc(kdc_active_realm, request, s_flags, &server,
                            &status);
    if (errcode != 0)
        goto cleanup;
    sprinc = server->princ;
     
    krb5_free_principal(kdc_context, request->server);
    request->server = NULL;
    errcode = krb5_copy_principal(kdc_context, server->princ,
                                  &request->server);
    if (errcode != 0) {
        status = "COPYING RESOLVED SERVER";
        goto cleanup;
    }

    if ((errcode = krb5_timeofday(kdc_context, &kdc_time))) {
        status = "TIME_OF_DAY";
        goto cleanup;
    }

    if ((retval = validate_tgs_request(kdc_active_realm,
                                       request, *server, header_ticket,
                                       kdc_time, &status, &e_data))) {
        if (!status)
            status = "UNKNOWN_REASON";
        errcode = retval + ERROR_TABLE_BASE_krb5;
        goto cleanup;
    }

    if (!is_local_principal(kdc_active_realm, header_enc_tkt->client))
        setflag(c_flags, KRB5_KDB_FLAG_CROSS_REALM);

    is_referral = krb5_is_tgs_principal(server->princ) &&
        !krb5_principal_compare(kdc_context, tgs_server, server->princ);

     
    errcode = kdc_process_s4u2self_req(kdc_active_realm,
                                       request,
                                       header_enc_tkt->client,
                                       server,
                                       subkey,
                                       header_enc_tkt->session,
                                       kdc_time,
                                       &s4u_x509_user,
                                       &client,
                                       &status);
    if (errcode)
        goto cleanup;
    if (s4u_x509_user != NULL)
        setflag(c_flags, KRB5_KDB_FLAG_PROTOCOL_TRANSITION);

    errcode = decrypt_2ndtkt(kdc_active_realm, request, c_flags,
                             &stkt_server, &status);
    if (errcode)
        goto cleanup;

    if (isflagset(request->kdc_options, KDC_OPT_CNAME_IN_ADDL_TKT)) {
         
        errcode = kdc_process_s4u2proxy_req(kdc_active_realm,
                                            request,
                                            request->second_ticket[st_idx]->enc_part2,
                                            stkt_server,
                                            header_ticket->enc_part2->client,
                                            request->server,
                                            &status);
        if (errcode)
            goto cleanup;

        setflag(c_flags, KRB5_KDB_FLAG_CONSTRAINED_DELEGATION);

        assert(krb5_is_tgs_principal(header_ticket->server));

        assert(client == NULL);  
        client = stkt_server;
        stkt_server = NULL;
    } else if (request->kdc_options & KDC_OPT_ENC_TKT_IN_SKEY) {
        krb5_db_free_principal(kdc_context, stkt_server);
        stkt_server = NULL;
    } else
        assert(stkt_server == NULL);

    errcode = gen_session_key(kdc_active_realm, request, server, &session_key,
                              &status);
    if (errcode)
        goto cleanup;

     

    if (isflagset(c_flags, KRB5_KDB_FLAG_CONSTRAINED_DELEGATION))
        subject_tkt = request->second_ticket[st_idx]->enc_part2;
    else
        subject_tkt = header_enc_tkt;
    authtime = subject_tkt->times.authtime;

    if (is_referral)
        ticket_reply.server = server->princ;
    else
        ticket_reply.server = request->server;  

    enc_tkt_reply.flags = 0;
    enc_tkt_reply.times.starttime = 0;

    if (isflagset(server->attributes, KRB5_KDB_OK_AS_DELEGATE))
        setflag(enc_tkt_reply.flags, TKT_FLG_OK_AS_DELEGATE);

     
    if (!(header_enc_tkt->times.starttime))
        header_enc_tkt->times.starttime = authtime;
    setflag(enc_tkt_reply.flags, TKT_FLG_ENC_PA_REP);

     

    enc_tkt_reply.caddrs = header_enc_tkt->caddrs;
     
    reply_encpart.caddrs = 0; 
    reply_encpart.enc_padata = NULL;

     

    if (isflagset(request->kdc_options, KDC_OPT_FORWARDABLE)) {
        setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDABLE);

        if (isflagset(c_flags, KRB5_KDB_FLAG_PROTOCOL_TRANSITION)) {
             
            if (client != NULL &&
                isflagset(client->attributes, KRB5_KDB_DISALLOW_FORWARDABLE))
                clear(enc_tkt_reply.flags, TKT_FLG_FORWARDABLE);
             
            else if (!isflagset(header_enc_tkt->flags, TKT_FLG_FORWARDABLE))
                clear(enc_tkt_reply.flags, TKT_FLG_FORWARDABLE);
             
            else if (!is_referral &&
                     !isflagset(server->attributes,
                                KRB5_KDB_OK_TO_AUTH_AS_DELEGATE))
                clear(enc_tkt_reply.flags, TKT_FLG_FORWARDABLE);
        }
    }

    if (isflagset(request->kdc_options, KDC_OPT_FORWARDED)) {
        setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDED);

         

        enc_tkt_reply.caddrs = request->addresses;
        reply_encpart.caddrs = request->addresses;
    }
    if (isflagset(header_enc_tkt->flags, TKT_FLG_FORWARDED))
        setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDED);

    if (isflagset(request->kdc_options, KDC_OPT_PROXIABLE))
        setflag(enc_tkt_reply.flags, TKT_FLG_PROXIABLE);

    if (isflagset(request->kdc_options, KDC_OPT_PROXY)) {
        setflag(enc_tkt_reply.flags, TKT_FLG_PROXY);

         

        enc_tkt_reply.caddrs = request->addresses;
        reply_encpart.caddrs = request->addresses;
    }

    if (isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE))
        setflag(enc_tkt_reply.flags, TKT_FLG_MAY_POSTDATE);

    if (isflagset(request->kdc_options, KDC_OPT_POSTDATED)) {
        setflag(enc_tkt_reply.flags, TKT_FLG_POSTDATED);
        setflag(enc_tkt_reply.flags, TKT_FLG_INVALID);
        enc_tkt_reply.times.starttime = request->from;
    } else
        enc_tkt_reply.times.starttime = kdc_time;

    if (isflagset(request->kdc_options, KDC_OPT_VALIDATE)) {
        assert(isflagset(c_flags, KRB5_KDB_FLAGS_S4U) == 0);
         
        ticket_reply = *(header_ticket);
        enc_tkt_reply = *(header_ticket->enc_part2);
        enc_tkt_reply.authorization_data = NULL;
        clear(enc_tkt_reply.flags, TKT_FLG_INVALID);
    }

    if (isflagset(request->kdc_options, KDC_OPT_RENEW)) {
        krb5_deltat old_life;

        assert(isflagset(c_flags, KRB5_KDB_FLAGS_S4U) == 0);
         
        ticket_reply = *(header_ticket);
        enc_tkt_reply = *(header_ticket->enc_part2);
        enc_tkt_reply.authorization_data = NULL;

        old_life = enc_tkt_reply.times.endtime - enc_tkt_reply.times.starttime;

        enc_tkt_reply.times.starttime = kdc_time;
        enc_tkt_reply.times.endtime =
            min(header_ticket->enc_part2->times.renew_till,
                kdc_time + old_life);
    } else {
         
        enc_tkt_reply.times.starttime = kdc_time;

        kdc_get_ticket_endtime(kdc_active_realm, enc_tkt_reply.times.starttime,
                               header_enc_tkt->times.endtime, request->till,
                               client, server, &enc_tkt_reply.times.endtime);

        if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE_OK) &&
            (enc_tkt_reply.times.endtime < request->till) &&
            isflagset(header_enc_tkt->flags, TKT_FLG_RENEWABLE)) {
            setflag(request->kdc_options, KDC_OPT_RENEWABLE);
            request->rtime =
                min(request->till, header_enc_tkt->times.renew_till);
        }
    }
    rtime = (request->rtime == 0) ? kdc_infinity : request->rtime;

    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE)) {
         
        setflag(enc_tkt_reply.flags, TKT_FLG_RENEWABLE);
        enc_tkt_reply.times.renew_till =
            min(rtime,
                min(header_enc_tkt->times.renew_till,
                    enc_tkt_reply.times.starttime +
                    min(server->max_renewable_life,
                        max_renewable_life_for_realm)));
    } else {
        enc_tkt_reply.times.renew_till = 0;
    }
    if (isflagset(header_enc_tkt->flags, TKT_FLG_ANONYMOUS))
        setflag(enc_tkt_reply.flags, TKT_FLG_ANONYMOUS);
     
    enc_tkt_reply.times.authtime = authtime;

     
    if (isflagset(header_enc_tkt->flags, TKT_FLG_PRE_AUTH))
        setflag(enc_tkt_reply.flags, TKT_FLG_PRE_AUTH);

    if (isflagset(header_enc_tkt->flags, TKT_FLG_HW_AUTH))
        setflag(enc_tkt_reply.flags, TKT_FLG_HW_AUTH);

     
    if (enc_tkt_reply.times.starttime == enc_tkt_reply.times.authtime)
        enc_tkt_reply.times.starttime = 0;

    if (isflagset(c_flags, KRB5_KDB_FLAG_PROTOCOL_TRANSITION)) {
        altcprinc = s4u_x509_user->user_id.user;
    } else if (isflagset(c_flags, KRB5_KDB_FLAG_CONSTRAINED_DELEGATION)) {
        altcprinc = subject_tkt->client;
    } else {
        altcprinc = NULL;
    }
    if (isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY)) {
        krb5_enc_tkt_part *t2enc = request->second_ticket[st_idx]->enc_part2;
        encrypting_key = *(t2enc->session);
    } else {
         
        if ((errcode = krb5_dbe_find_enctype(kdc_context, server,
                                             -1,  
                                             -1,  
                                             0,   
                                             &server_key))) {
            status = "FINDING_SERVER_KEY";
            goto cleanup;
        }

         
        if ((errcode = krb5_dbe_decrypt_key_data(kdc_context, NULL,
                                                 server_key, &encrypting_key,
                                                 NULL))) {
            status = "DECRYPT_SERVER_KEY";
            goto cleanup;
        }
    }

    if (isflagset(c_flags, KRB5_KDB_FLAG_CONSTRAINED_DELEGATION)) {
         
        clear(server->attributes, KRB5_KDB_NO_AUTH_DATA_REQUIRED);
    }
    if (isflagset(server->attributes, KRB5_KDB_NO_AUTH_DATA_REQUIRED) == 0) {
         
        if (!isflagset(c_flags, KRB5_KDB_FLAGS_S4U)) {
             
            setflag(c_flags, KRB5_KDB_FLAG_INCLUDE_PAC);
             
            setflag(c_flags, KRB5_KDB_FLAG_MAP_PRINCIPALS);

            assert(client == NULL);  

            errcode = krb5_db_get_principal(kdc_context, subject_tkt->client,
                                            c_flags, &client);
        }
    }

    if (isflagset(c_flags, KRB5_KDB_FLAG_PROTOCOL_TRANSITION) &&
        !isflagset(c_flags, KRB5_KDB_FLAG_CROSS_REALM))
        enc_tkt_reply.client = s4u_x509_user->user_id.user;
    else
        enc_tkt_reply.client = subject_tkt->client;

    enc_tkt_reply.session = &session_key;
    enc_tkt_reply.transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
    enc_tkt_reply.transited.tr_contents = empty_string;  

    errcode = handle_authdata(kdc_context, c_flags, client, server, krbtgt,
                              subkey != NULL ? subkey :
                              header_ticket->enc_part2->session,
                              &encrypting_key,  
                              tgskey,
                              pkt,
                              request,
                              s4u_x509_user ?
                              s4u_x509_user->user_id.user : NULL,
                              subject_tkt,
                              &enc_tkt_reply);
    if (errcode) {
        krb5_klog_syslog(LOG_INFO, _("TGS_REQ : handle_authdata (%d)"),
                         errcode);
        status = "HANDLE_AUTHDATA";
        goto cleanup;
    }


     
     
    if (krb5_realm_compare(kdc_context, header_ticket->server, tgs_server) ||
        krb5_realm_compare(kdc_context, header_ticket->server,
                           enc_tkt_reply.client)) {
         
        enc_tkt_reply.transited = header_enc_tkt->transited;
    } else {
         
         
        if (header_enc_tkt->transited.tr_type !=
            KRB5_DOMAIN_X500_COMPRESS) {
            status = "BAD_TRTYPE";
            errcode = KRB5KDC_ERR_TRTYPE_NOSUPP;
            goto cleanup;
        }
        enc_tkt_transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
        enc_tkt_transited.magic = 0;
        enc_tkt_transited.tr_contents.magic = 0;
        enc_tkt_transited.tr_contents.data = 0;
        enc_tkt_transited.tr_contents.length = 0;
        enc_tkt_reply.transited = enc_tkt_transited;
        if ((errcode =
             add_to_transited(&header_enc_tkt->transited.tr_contents,
                              &enc_tkt_reply.transited.tr_contents,
                              header_ticket->server,
                              enc_tkt_reply.client,
                              request->server))) {
            status = "ADD_TR_FAIL";
            goto cleanup;
        }
        newtransited = 1;
    }
    if (isflagset(c_flags, KRB5_KDB_FLAG_CROSS_REALM)) {
        errcode = validate_transit_path(kdc_context, header_enc_tkt->client,
                                        server, krbtgt);
        if (errcode) {
            status = "NON_TRANSITIVE";
            goto cleanup;
        }
    }
    if (!isflagset (request->kdc_options, KDC_OPT_DISABLE_TRANSITED_CHECK)) {
        errcode = kdc_check_transited_list (kdc_active_realm,
                                            &enc_tkt_reply.transited.tr_contents,
                                            krb5_princ_realm (kdc_context, header_enc_tkt->client),
                                            krb5_princ_realm (kdc_context, request->server));
        if (errcode == 0) {
            setflag (enc_tkt_reply.flags, TKT_FLG_TRANSIT_POLICY_CHECKED);
        } else {
            log_tgs_badtrans(kdc_context, cprinc, sprinc,
                             &enc_tkt_reply.transited.tr_contents, errcode);
        }
    } else
        krb5_klog_syslog(LOG_INFO, _("not checking transit path"));
    if (reject_bad_transit
        && !isflagset (enc_tkt_reply.flags, TKT_FLG_TRANSIT_POLICY_CHECKED)) {
        errcode = KRB5KDC_ERR_POLICY;
        status = "BAD_TRANSIT";
        goto cleanup;
    }

    ticket_reply.enc_part2 = &enc_tkt_reply;

     
    if (isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY)) {
         
        krb5_enc_tkt_part *t2enc = request->second_ticket[st_idx]->enc_part2;
        krb5_principal client2 = t2enc->client;
        if (!krb5_principal_compare(kdc_context, request->server, client2)) {
            altcprinc = client2;
            errcode = KRB5KDC_ERR_SERVER_NOMATCH;
            status = "2ND_TKT_MISMATCH";
            goto cleanup;
        }

        ticket_kvno = 0;
        ticket_reply.enc_part.enctype = t2enc->session->enctype;
        st_idx++;
    } else {
        ticket_kvno = server_key->key_data_kvno;
    }

    errcode = krb5_encrypt_tkt_part(kdc_context, &encrypting_key,
                                    &ticket_reply);
    if (!isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY))
        krb5_free_keyblock_contents(kdc_context, &encrypting_key);
    if (errcode) {
        status = "TKT_ENCRYPT";
        goto cleanup;
    }
    ticket_reply.enc_part.kvno = ticket_kvno;
     
    reply.msg_type = KRB5_TGS_REP;
    if (isflagset(c_flags, KRB5_KDB_FLAG_PROTOCOL_TRANSITION) &&
        krb5int_find_pa_data(kdc_context, request->padata,
                             KRB5_PADATA_S4U_X509_USER) != NULL) {
        errcode = kdc_make_s4u2self_rep(kdc_context,
                                        subkey,
                                        header_ticket->enc_part2->session,
                                        s4u_x509_user,
                                        &reply,
                                        &reply_encpart);
        if (errcode) {
            status = "KDC_RETURN_S4U2SELF_PADATA";
            goto cleanup;
        }
    }

    reply.client = enc_tkt_reply.client;
    reply.enc_part.kvno = 0; 
    reply.ticket = &ticket_reply;

    reply_encpart.session = &session_key;
    reply_encpart.nonce = request->nonce;

     
    reply_encpart.times = enc_tkt_reply.times;

     
    if (enc_tkt_reply.times.starttime == enc_tkt_reply.times.authtime)
        enc_tkt_reply.times.starttime = 0;

    nolrentry.lr_type = KRB5_LRQ_NONE;
    nolrentry.value = 0;
    nolrarray[0] = &nolrentry;
    nolrarray[1] = 0;
    reply_encpart.last_req = nolrarray;         
    reply_encpart.key_exp = 0; 
    reply_encpart.flags = enc_tkt_reply.flags;
    reply_encpart.server = ticket_reply.server;

     
    reply.enc_part.enctype = subkey ? subkey->enctype :
        header_ticket->enc_part2->session->enctype;
    errcode  = kdc_fast_response_handle_padata(state, request, &reply,
                                               subkey ? subkey->enctype : header_ticket->enc_part2->session->enctype);
    if (errcode !=0 ) {
        status = "Preparing FAST padata";
        goto cleanup;
    }
    errcode =kdc_fast_handle_reply_key(state,
                                       subkey?subkey:header_ticket->enc_part2->session, &reply_key);
    if (errcode) {
        status  = "generating reply key";
        goto cleanup;
    }
    errcode = return_enc_padata(kdc_context, pkt, request,
                                reply_key, server, &reply_encpart,
                                is_referral &&
                                isflagset(s_flags,
                                          KRB5_KDB_FLAG_CANONICALIZE));
    if (errcode) {
        status = "KDC_RETURN_ENC_PADATA";
        goto cleanup;
    }

    errcode = krb5_encode_kdc_rep(kdc_context, KRB5_TGS_REP, &reply_encpart,
                                  subkey ? 1 : 0,
                                  reply_key,
                                  &reply, response);
    if (errcode) {
        status = "ENCODE_KDC_REP";
    } else {
        status = "ISSUE";
    }

    memset(ticket_reply.enc_part.ciphertext.data, 0,
           ticket_reply.enc_part.ciphertext.length);
    free(ticket_reply.enc_part.ciphertext.data);
     
    memset(reply.enc_part.ciphertext.data, 0,
           reply.enc_part.ciphertext.length);
    free(reply.enc_part.ciphertext.data);

cleanup:
    assert(status != NULL);
    if (reply_key)
        krb5_free_keyblock(kdc_context, reply_key);
    if (errcode)
        emsg = krb5_get_error_message (kdc_context, errcode);
    log_tgs_req(kdc_context, from, request, &reply, cprinc,
                sprinc, altcprinc, authtime,
                c_flags, status, errcode, emsg);
    if (errcode) {
        krb5_free_error_message (kdc_context, emsg);
        emsg = NULL;
    }

    if (errcode) {
        int got_err = 0;
        if (status == 0) {
            status = krb5_get_error_message (kdc_context, errcode);
            got_err = 1;
        }
        errcode -= ERROR_TABLE_BASE_krb5;
        if (errcode < 0 || errcode > 128)
            errcode = KRB_ERR_GENERIC;

        retval = prepare_error_tgs(state, request, header_ticket, errcode,
                                   (server != NULL) ? server->princ : NULL,
                                   response, status, e_data);
        if (got_err) {
            krb5_free_error_message (kdc_context, status);
            status = 0;
        }
    }

    if (header_ticket != NULL)
        krb5_free_ticket(kdc_context, header_ticket);
    if (request != NULL)
        krb5_free_kdc_req(kdc_context, request);
    if (state)
        kdc_free_rstate(state);
    krb5_db_free_principal(kdc_context, server);
    krb5_db_free_principal(kdc_context, krbtgt);
    krb5_db_free_principal(kdc_context, client);
    if (session_key.contents != NULL)
        krb5_free_keyblock_contents(kdc_context, &session_key);
    if (newtransited)
        free(enc_tkt_reply.transited.tr_contents.data);
    if (s4u_x509_user != NULL)
        krb5_free_pa_s4u_x509_user(kdc_context, s4u_x509_user);
    if (kdc_issued_auth_data != NULL)
        krb5_free_authdata(kdc_context, kdc_issued_auth_data);
    if (subkey != NULL)
        krb5_free_keyblock(kdc_context, subkey);
    if (tgskey != NULL)
        krb5_free_keyblock(kdc_context, tgskey);
    if (reply.padata)
        krb5_free_pa_data(kdc_context, reply.padata);
    if (reply_encpart.enc_padata)
        krb5_free_pa_data(kdc_context, reply_encpart.enc_padata);
    if (enc_tkt_reply.authorization_data != NULL)
        krb5_free_authdata(kdc_context, enc_tkt_reply.authorization_data);
    krb5_free_pa_data(kdc_context, e_data);

    return retval;
}