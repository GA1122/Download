process_as_req(krb5_kdc_req *request, krb5_data *req_pkt,
               const krb5_fulladdr *local_addr,
               const krb5_fulladdr *remote_addr, kdc_realm_t *kdc_active_realm,
               verto_ctx *vctx, loop_respond_fn respond, void *arg)
{
    krb5_error_code errcode;
    unsigned int s_flags = 0;
    krb5_data encoded_req_body;
    krb5_enctype useenctype;
    struct as_req_state *state;
    krb5_audit_state *au_state = NULL;

    state = k5alloc(sizeof(*state), &errcode);
    if (state == NULL) {
        (*respond)(arg, errcode, NULL);
        return;
    }
    state->respond = respond;
    state->arg = arg;
    state->request = request;
    state->req_pkt = req_pkt;
    state->local_addr = local_addr;
    state->remote_addr = remote_addr;
    state->active_realm = kdc_active_realm;

    errcode = kdc_make_rstate(kdc_active_realm, &state->rstate);
    if (errcode != 0) {
        (*respond)(arg, errcode, NULL);
        free(state);
        return;
    }

     
    errcode = kau_init_kdc_req(kdc_context, state->request, remote_addr,
                               &au_state);
    if (errcode) {
        (*respond)(arg, errcode, NULL);
        kdc_free_rstate(state->rstate);
        free(state);
        return;
    }
    state->au_state = au_state;

    if (state->request->msg_type != KRB5_AS_REQ) {
        state->status = "VALIDATE_MESSAGE_TYPE";
        errcode = KRB5_BADMSGTYPE;
        goto errout;
    }

     
    kau_as_req(kdc_context, TRUE, au_state);

    if (fetch_asn1_field((unsigned char *) req_pkt->data,
                         1, 4, &encoded_req_body) != 0) {
        errcode = ASN1_BAD_ID;
        state->status = "FETCH_REQ_BODY";
        goto errout;
    }
    errcode = kdc_find_fast(&state->request, &encoded_req_body, NULL, NULL,
                            state->rstate, &state->inner_body);
    if (errcode) {
        state->status = "FIND_FAST";
        goto errout;
    }
    if (state->inner_body == NULL) {
         
        errcode = krb5_copy_data(kdc_context, &encoded_req_body,
                                 &state->inner_body);
        if (errcode) {
            state->status = "COPY_REQ_BODY";
            goto errout;
        }
    }
    au_state->request = state->request;
    state->rock.request = state->request;
    state->rock.inner_body = state->inner_body;
    state->rock.rstate = state->rstate;
    state->rock.vctx = vctx;
    state->rock.auth_indicators = &state->auth_indicators;
    if (!state->request->client) {
        state->status = "NULL_CLIENT";
        errcode = KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;
        goto errout;
    }
    if ((errcode = krb5_unparse_name(kdc_context,
                                     state->request->client,
                                     &state->cname))) {
        state->status = "UNPARSE_CLIENT";
        goto errout;
    }
    limit_string(state->cname);

    if (!state->request->server) {
        state->status = "NULL_SERVER";
        errcode = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
        goto errout;
    }
    if ((errcode = krb5_unparse_name(kdc_context,
                                     state->request->server,
                                     &state->sname))) {
        state->status = "UNPARSE_SERVER";
        goto errout;
    }
    limit_string(state->sname);

     
    setflag(state->c_flags, KRB5_KDB_FLAG_CLIENT_REFERRALS_ONLY);
     
    if (isflagset(state->request->kdc_options, KDC_OPT_CANONICALIZE) ||
        state->request->client->type == KRB5_NT_ENTERPRISE_PRINCIPAL) {
        setflag(state->c_flags, KRB5_KDB_FLAG_CANONICALIZE);
        setflag(state->c_flags, KRB5_KDB_FLAG_ALIAS_OK);
    }
    if (include_pac_p(kdc_context, state->request)) {
        setflag(state->c_flags, KRB5_KDB_FLAG_INCLUDE_PAC);
    }
    errcode = krb5_db_get_principal(kdc_context, state->request->client,
                                    state->c_flags, &state->client);
    if (errcode == KRB5_KDB_CANTLOCK_DB)
        errcode = KRB5KDC_ERR_SVC_UNAVAILABLE;
    if (errcode == KRB5_KDB_NOENTRY) {
        state->status = "CLIENT_NOT_FOUND";
        if (vague_errors)
            errcode = KRB5KRB_ERR_GENERIC;
        else
            errcode = KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;
        goto errout;
    } else if (errcode) {
        state->status = "LOOKING_UP_CLIENT";
        goto errout;
    }
    state->rock.client = state->client;

     
    if (!is_local_principal(kdc_active_realm, state->client->princ)) {
         
        state->status = "REFERRAL";
        au_state->cl_realm = &state->client->princ->realm;
        errcode = KRB5KDC_ERR_WRONG_REALM;
        goto errout;
    }

    au_state->stage = SRVC_PRINC;

    s_flags = 0;
    setflag(s_flags, KRB5_KDB_FLAG_ALIAS_OK);
    if (isflagset(state->request->kdc_options, KDC_OPT_CANONICALIZE)) {
        setflag(s_flags, KRB5_KDB_FLAG_CANONICALIZE);
    }
    errcode = krb5_db_get_principal(kdc_context, state->request->server,
                                    s_flags, &state->server);
    if (errcode == KRB5_KDB_CANTLOCK_DB)
        errcode = KRB5KDC_ERR_SVC_UNAVAILABLE;
    if (errcode == KRB5_KDB_NOENTRY) {
        state->status = "SERVER_NOT_FOUND";
        errcode = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
        goto errout;
    } else if (errcode) {
        state->status = "LOOKING_UP_SERVER";
        goto errout;
    }

    errcode = get_local_tgt(kdc_context, &state->request->server->realm,
                            state->server, &state->local_tgt,
                            &state->local_tgt_storage);
    if (errcode) {
        state->status = "GET_LOCAL_TGT";
        goto errout;
    }

    au_state->stage = VALIDATE_POL;

    if ((errcode = krb5_timeofday(kdc_context, &state->kdc_time))) {
        state->status = "TIMEOFDAY";
        goto errout;
    }
    state->authtime = state->kdc_time;  

    if ((errcode = validate_as_request(kdc_active_realm,
                                       state->request, *state->client,
                                       *state->server, state->kdc_time,
                                       &state->status, &state->e_data))) {
        if (!state->status)
            state->status = "UNKNOWN_REASON";
        errcode += ERROR_TABLE_BASE_krb5;
        goto errout;
    }

    au_state->stage = ISSUE_TKT;

     
    if ((useenctype = select_session_keytype(kdc_active_realm, state->server,
                                             state->request->nktypes,
                                             state->request->ktype)) == 0) {
         
        state->status = "BAD_ENCRYPTION_TYPE";
        errcode = KRB5KDC_ERR_ETYPE_NOSUPP;
        goto errout;
    }

    if ((errcode = krb5_c_make_random_key(kdc_context, useenctype,
                                          &state->session_key))) {
        state->status = "MAKE_RANDOM_KEY";
        goto errout;
    }

     
    if (isflagset(s_flags, KRB5_KDB_FLAG_CANONICALIZE) &&
        krb5_is_tgs_principal(state->request->server) &&
        krb5_is_tgs_principal(state->server->princ)) {
        state->ticket_reply.server = state->server->princ;
    } else {
        state->ticket_reply.server = state->request->server;
    }

     
    state->enc_tkt_reply.flags = OPTS2FLAGS(state->request->kdc_options);
    state->enc_tkt_reply.times.authtime = state->authtime;

    setflag(state->enc_tkt_reply.flags, TKT_FLG_INITIAL);
    setflag(state->enc_tkt_reply.flags, TKT_FLG_ENC_PA_REP);

     

    state->enc_tkt_reply.session = &state->session_key;
    if (isflagset(state->c_flags, KRB5_KDB_FLAG_CANONICALIZE)) {
        state->client_princ = *(state->client->princ);
    } else {
        state->client_princ = *(state->request->client);
         
        state->client_princ.realm = state->client->princ->realm;
    }
    state->enc_tkt_reply.client = &state->client_princ;
    state->enc_tkt_reply.transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
    state->enc_tkt_reply.transited.tr_contents = empty_string;

    if (isflagset(state->request->kdc_options, KDC_OPT_POSTDATED)) {
        setflag(state->enc_tkt_reply.flags, TKT_FLG_INVALID);
        state->enc_tkt_reply.times.starttime = state->request->from;
    } else
        state->enc_tkt_reply.times.starttime = state->kdc_time;

    kdc_get_ticket_endtime(kdc_active_realm,
                           state->enc_tkt_reply.times.starttime,
                           kdc_infinity, state->request->till, state->client,
                           state->server, &state->enc_tkt_reply.times.endtime);

    kdc_get_ticket_renewtime(kdc_active_realm, state->request, NULL,
                             state->client, state->server,
                             &state->enc_tkt_reply);

     
    if (state->enc_tkt_reply.times.starttime ==
        state->enc_tkt_reply.times.authtime)
        state->enc_tkt_reply.times.starttime = 0;

    state->enc_tkt_reply.caddrs = state->request->addresses;
    state->enc_tkt_reply.authorization_data = 0;

     
    if (isflagset(state->request->kdc_options, KDC_OPT_REQUEST_ANONYMOUS)) {
        if (!krb5_principal_compare_any_realm(kdc_context,
                                              state->request->client,
                                              krb5_anonymous_principal())) {
            errcode = KRB5KDC_ERR_BADOPTION;
             
            state->status = "VALIDATE_ANONYMOUS_PRINCIPAL";
            goto errout;
        }
        krb5_free_principal(kdc_context, state->request->client);
        state->request->client = NULL;
        errcode = krb5_copy_principal(kdc_context, krb5_anonymous_principal(),
                                      &state->request->client);
        if (errcode) {
            state->status = "COPY_ANONYMOUS_PRINCIPAL";
            goto errout;
        }
        state->enc_tkt_reply.client = state->request->client;
        setflag(state->client->attributes, KRB5_KDB_REQUIRES_PRE_AUTH);
    }

    errcode = select_client_key(kdc_context, state->client,
                                state->request->ktype, state->request->nktypes,
                                &state->client_keyblock, &state->client_key);
    if (errcode) {
        state->status = "DECRYPT_CLIENT_KEY";
        goto errout;
    }
    if (state->client_key != NULL) {
        state->rock.client_key = state->client_key;
        state->rock.client_keyblock = &state->client_keyblock;
    }

    errcode = kdc_fast_read_cookie(kdc_context, state->rstate, state->request,
                                   state->local_tgt);
    if (errcode) {
        state->status = "READ_COOKIE";
        goto errout;
    }

     
    if (state->request->padata) {
        check_padata(kdc_context, &state->rock, state->req_pkt,
                     state->request, &state->enc_tkt_reply, &state->pa_context,
                     &state->e_data, &state->typed_e_data, finish_preauth,
                     state);
    } else
        finish_preauth(state, 0);
    return;

errout:
    finish_process_as_req(state, errcode);
}