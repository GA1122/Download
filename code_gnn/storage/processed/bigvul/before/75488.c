krb5_get_self_cred_from_kdc(krb5_context context,
                            krb5_flags options,
                            krb5_ccache ccache,
                            krb5_creds *in_creds,
                            krb5_data *subject_cert,
                            krb5_data *user_realm,
                            krb5_creds **out_creds)
{
    krb5_error_code code;
    krb5_principal tgs = NULL, eprinc = NULL;
    krb5_principal_data sprinc;
    krb5_creds tgtq, s4u_creds, *tgt = NULL, *tgtptr;
    krb5_creds *referral_tgts[KRB5_REFERRAL_MAXHOPS];
    krb5_pa_s4u_x509_user s4u_user;
    int referral_count = 0, i;
    krb5_flags kdcopt;

    memset(&tgtq, 0, sizeof(tgtq));
    memset(referral_tgts, 0, sizeof(referral_tgts));
    *out_creds = NULL;

    memset(&s4u_user, 0, sizeof(s4u_user));

    if (in_creds->client != NULL && in_creds->client->length > 0) {
        if (in_creds->client->type == KRB5_NT_ENTERPRISE_PRINCIPAL) {
            code = krb5_build_principal_ext(context,
                                            &s4u_user.user_id.user,
                                            user_realm->length,
                                            user_realm->data,
                                            in_creds->client->data[0].length,
                                            in_creds->client->data[0].data,
                                            0);
            if (code != 0)
                goto cleanup;
            s4u_user.user_id.user->type = KRB5_NT_ENTERPRISE_PRINCIPAL;
        } else {
            code = krb5_copy_principal(context,
                                       in_creds->client,
                                       &s4u_user.user_id.user);
            if (code != 0)
                goto cleanup;
        }
    } else {
        code = krb5_build_principal_ext(context, &s4u_user.user_id.user,
                                        user_realm->length,
                                        user_realm->data);
        if (code != 0)
            goto cleanup;
        s4u_user.user_id.user->type = KRB5_NT_ENTERPRISE_PRINCIPAL;
    }
    if (subject_cert != NULL)
        s4u_user.user_id.subject_cert = *subject_cert;
    s4u_user.user_id.options = KRB5_S4U_OPTS_USE_REPLY_KEY_USAGE;

     
    code = krb5int_tgtname(context, user_realm, &in_creds->server->realm,
                           &tgs);
    if (code != 0)
        goto cleanup;

    tgtq.client = in_creds->server;
    tgtq.server = tgs;

    code = krb5_get_credentials(context, options, ccache, &tgtq, &tgt);
    if (code != 0)
        goto cleanup;

    tgtptr = tgt;

     
    code = convert_to_enterprise(context, in_creds->server, &eprinc);
    if (code != 0)
        goto cleanup;

     
    s4u_creds = *in_creds;
    s4u_creds.client = in_creds->server;

     
    kdcopt = 0;
    if (options & KRB5_GC_CANONICALIZE)
        kdcopt |= KDC_OPT_CANONICALIZE;
    if (options & KRB5_GC_FORWARDABLE)
        kdcopt |= KDC_OPT_FORWARDABLE;
    if (options & KRB5_GC_NO_TRANSIT_CHECK)
        kdcopt |= KDC_OPT_DISABLE_TRANSITED_CHECK;

    for (referral_count = 0;
         referral_count < KRB5_REFERRAL_MAXHOPS;
         referral_count++)
    {
        krb5_pa_data **in_padata = NULL;
        krb5_pa_data **out_padata = NULL;
        krb5_pa_data **enc_padata = NULL;
        krb5_keyblock *subkey = NULL;

        if (s4u_user.user_id.user != NULL && s4u_user.user_id.user->length) {
            in_padata = calloc(2, sizeof(krb5_pa_data *));
            if (in_padata == NULL) {
                code = ENOMEM;
                goto cleanup;
            }
            code = build_pa_for_user(context,
                                     tgtptr,
                                     &s4u_user.user_id, &in_padata[0]);
            if (code != 0) {
                krb5_free_pa_data(context, in_padata);
                goto cleanup;
            }
        }

        if (data_eq(tgtptr->server->data[1], in_creds->server->realm)) {
             
            s4u_creds.server = in_creds->server;
        } else {
             
            sprinc = *eprinc;
            sprinc.realm = tgtptr->server->data[1];
            s4u_creds.server = &sprinc;
        }

        code = krb5_get_cred_via_tkt_ext(context, tgtptr,
                                         KDC_OPT_CANONICALIZE |
                                         FLAGS2OPTS(tgtptr->ticket_flags) |
                                         kdcopt,
                                         tgtptr->addresses,
                                         in_padata, &s4u_creds,
                                         build_pa_s4u_x509_user, &s4u_user,
                                         &out_padata, &enc_padata,
                                         out_creds, &subkey);
        if (code != 0) {
            krb5_free_checksum_contents(context, &s4u_user.cksum);
            krb5_free_pa_data(context, in_padata);
            goto cleanup;
        }

        code = verify_s4u2self_reply(context, subkey, &s4u_user,
                                     out_padata, enc_padata);

        krb5_free_checksum_contents(context, &s4u_user.cksum);
        krb5_free_pa_data(context, in_padata);
        krb5_free_pa_data(context, out_padata);
        krb5_free_pa_data(context, enc_padata);
        krb5_free_keyblock(context, subkey);

        if (code != 0)
            goto cleanup;

        if (krb5_principal_compare(context,
                                   in_creds->server,
                                   (*out_creds)->server)) {
            code = 0;
            goto cleanup;
        } else if (IS_TGS_PRINC((*out_creds)->server)) {
            krb5_data *r1 = &tgtptr->server->data[1];
            krb5_data *r2 = &(*out_creds)->server->data[1];

            if (data_eq(*r1, *r2)) {
                krb5_free_creds(context, *out_creds);
                *out_creds = NULL;
                code = KRB5_ERR_HOST_REALM_UNKNOWN;
                break;
            }
            for (i = 0; i < referral_count; i++) {
                if (krb5_principal_compare(context,
                                           (*out_creds)->server,
                                           referral_tgts[i]->server)) {
                    code = KRB5_KDC_UNREACH;
                    goto cleanup;
                }
            }

            tgtptr = *out_creds;
            referral_tgts[referral_count] = *out_creds;
            *out_creds = NULL;
        } else {
            krb5_free_creds(context, *out_creds);
            *out_creds = NULL;
            code = KRB5KRB_AP_WRONG_PRINC;  
            break;
        }
    }

cleanup:
    for (i = 0; i < KRB5_REFERRAL_MAXHOPS; i++) {
        if (referral_tgts[i] != NULL)
            krb5_free_creds(context, referral_tgts[i]);
    }
    krb5_free_principal(context, tgs);
    krb5_free_principal(context, eprinc);
    krb5_free_creds(context, tgt);
    krb5_free_principal(context, s4u_user.user_id.user);
    krb5_free_checksum_contents(context, &s4u_user.cksum);

    return code;
}