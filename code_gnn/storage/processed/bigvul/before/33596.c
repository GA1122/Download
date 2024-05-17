is_referral_req(kdc_realm_t *kdc_active_realm, krb5_kdc_req *request)
{
    krb5_boolean ret = FALSE;
    char *stype = NULL;
    char *ref_services = kdc_active_realm->realm_host_based_services;
    char *nonref_services = kdc_active_realm->realm_no_host_referral;

    if (!(request->kdc_options & KDC_OPT_CANONICALIZE))
        return FALSE;

    if (request->kdc_options & KDC_OPT_ENC_TKT_IN_SKEY)
        return FALSE;

    if (krb5_princ_size(kdc_context, request->server) != 2)
        return FALSE;

    stype = data2string(krb5_princ_component(kdc_context, request->server, 0));
    if (stype == NULL)
        return FALSE;
    switch (krb5_princ_type(kdc_context, request->server)) {
    case KRB5_NT_UNKNOWN:
         
        if (kdc_active_realm->realm_host_based_services != NULL) {
            if (!krb5_match_config_pattern(ref_services, stype) &&
                !krb5_match_config_pattern(ref_services, KRB5_CONF_ASTERISK))
                goto cleanup;
        } else
            goto cleanup;
         
    case KRB5_NT_SRV_HST:
    case KRB5_NT_SRV_INST:
         
        if (kdc_active_realm->realm_no_host_referral != NULL) {
            if (krb5_match_config_pattern(nonref_services, stype))
                goto cleanup;
            if (krb5_match_config_pattern(nonref_services, KRB5_CONF_ASTERISK))
                goto cleanup;
        }
        ret = TRUE;
        break;
    default:
        goto cleanup;
    }
cleanup:
    free(stype);
    return ret;
}