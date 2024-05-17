krb5_get_credentials_for_proxy(krb5_context context,
                               krb5_flags options,
                               krb5_ccache ccache,
                               krb5_creds *in_creds,
                               krb5_ticket *evidence_tkt,
                               krb5_creds **out_creds)
{
    krb5_error_code code;
    krb5_creds mcreds;
    krb5_creds *ncreds = NULL;
    krb5_flags fields;
    krb5_data *evidence_tkt_data = NULL;
    krb5_creds s4u_creds;

    *out_creds = NULL;

    if (in_creds == NULL || in_creds->client == NULL ||
        evidence_tkt == NULL || evidence_tkt->enc_part2 == NULL) {
        code = EINVAL;
        goto cleanup;
    }

     
    if (!krb5_principal_compare(context, evidence_tkt->enc_part2->client,
                                in_creds->client)) {
        code = EINVAL;
        goto cleanup;
    }

    if ((evidence_tkt->enc_part2->flags & TKT_FLG_FORWARDABLE) == 0) {
        code = KRB5_TKT_NOT_FORWARDABLE;
        goto cleanup;
    }

    code = krb5int_construct_matching_creds(context, options, in_creds,
                                            &mcreds, &fields);
    if (code != 0)
        goto cleanup;

    ncreds = calloc(1, sizeof(*ncreds));
    if (ncreds == NULL) {
        code = ENOMEM;
        goto cleanup;
    }
    ncreds->magic = KV5M_CRED;

    code = krb5_cc_retrieve_cred(context, ccache, fields, &mcreds, ncreds);
    if (code != 0) {
        free(ncreds);
        ncreds = in_creds;
    } else {
        *out_creds = ncreds;
    }

    if ((code != KRB5_CC_NOTFOUND && code != KRB5_CC_NOT_KTYPE)
        || options & KRB5_GC_CACHED)
        goto cleanup;

    code = encode_krb5_ticket(evidence_tkt, &evidence_tkt_data);
    if (code != 0)
        goto cleanup;

    s4u_creds = *in_creds;
    s4u_creds.client = evidence_tkt->server;
    s4u_creds.second_ticket = *evidence_tkt_data;

    code = krb5_get_credentials(context,
                                options | KRB5_GC_CONSTRAINED_DELEGATION,
                                ccache,
                                &s4u_creds,
                                out_creds);
    if (code != 0)
        goto cleanup;

     
    if (!krb5_principal_compare(context,
                                evidence_tkt->enc_part2->client,
                                (*out_creds)->client)) {
        code = KRB5_KDCREP_MODIFIED;
        goto cleanup;
    }

cleanup:
    if (*out_creds != NULL && code != 0) {
        krb5_free_creds(context, *out_creds);
        *out_creds = NULL;
    }
    if (evidence_tkt_data != NULL)
        krb5_free_data(context, evidence_tkt_data);

    return code;
}