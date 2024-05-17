kdc_process_s4u2self_req(kdc_realm_t *kdc_active_realm,
                         krb5_kdc_req *request,
                         krb5_const_principal client_princ,
                         const krb5_db_entry *server,
                         krb5_keyblock *tgs_subkey,
                         krb5_keyblock *tgs_session,
                         krb5_timestamp kdc_time,
                         krb5_pa_s4u_x509_user **s4u_x509_user,
                         krb5_db_entry **princ_ptr,
                         const char **status)
{
    krb5_error_code             code;
    krb5_pa_data                *pa_data;
    int                         flags;
    krb5_db_entry               *princ;

    *princ_ptr = NULL;

    pa_data = krb5int_find_pa_data(kdc_context,
                                   request->padata, KRB5_PADATA_S4U_X509_USER);
    if (pa_data != NULL) {
        code = kdc_process_s4u_x509_user(kdc_context,
                                         request,
                                         pa_data,
                                         tgs_subkey,
                                         tgs_session,
                                         s4u_x509_user,
                                         status);
        if (code != 0)
            return code;
    } else {
        pa_data = krb5int_find_pa_data(kdc_context,
                                       request->padata, KRB5_PADATA_FOR_USER);
        if (pa_data != NULL) {
            code = kdc_process_for_user(kdc_active_realm,
                                        pa_data,
                                        tgs_session,
                                        s4u_x509_user,
                                        status);
            if (code != 0)
                return code;
        } else
            return 0;
    }

     
    flags = 0;
    switch (krb5_princ_type(kdc_context, request->server)) {
    case KRB5_NT_SRV_HST:                    
        if (krb5_princ_size(kdc_context, request->server) == 2)
            flags |= KRB5_PRINCIPAL_COMPARE_IGNORE_REALM;
        break;
    case KRB5_NT_ENTERPRISE_PRINCIPAL:       
        flags |= KRB5_PRINCIPAL_COMPARE_ENTERPRISE;
        break;
    default:                                 
        break;
    }

    if (!krb5_principal_compare_flags(kdc_context,
                                      request->server,
                                      client_princ,
                                      flags)) {
        *status = "INVALID_S4U2SELF_REQUEST";
        return KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;  
    }

     
    if (request->kdc_options & AS_INVALID_OPTIONS) {
        *status = "INVALID AS OPTIONS";
        return KRB5KDC_ERR_BADOPTION;
    }

     
    if (is_local_principal(kdc_active_realm,
                           (*s4u_x509_user)->user_id.user)) {
        krb5_db_entry no_server;
        krb5_pa_data **e_data = NULL;

        code = krb5_db_get_principal(kdc_context,
                                     (*s4u_x509_user)->user_id.user,
                                     KRB5_KDB_FLAG_INCLUDE_PAC, &princ);
        if (code == KRB5_KDB_NOENTRY) {
            *status = "UNKNOWN_S4U2SELF_PRINCIPAL";
            return KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;
        } else if (code) {
            *status = "LOOKING_UP_S4U2SELF_PRINCIPAL";
            return code;  
        }

        memset(&no_server, 0, sizeof(no_server));

        code = validate_as_request(kdc_active_realm, request, *princ,
                                   no_server, kdc_time, status, &e_data);
        if (code) {
            krb5_db_free_principal(kdc_context, princ);
            krb5_free_pa_data(kdc_context, e_data);
            return code;
        }

        *princ_ptr = princ;
    }

    return 0;
}
