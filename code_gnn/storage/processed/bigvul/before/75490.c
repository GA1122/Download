verify_s4u2self_reply(krb5_context context,
                      krb5_keyblock *subkey,
                      krb5_pa_s4u_x509_user *req_s4u_user,
                      krb5_pa_data **rep_padata,
                      krb5_pa_data **enc_padata)
{
    krb5_error_code code;
    krb5_pa_data *rep_s4u_padata, *enc_s4u_padata;
    krb5_pa_s4u_x509_user *rep_s4u_user = NULL;
    krb5_data data, *datap = NULL;
    krb5_keyusage usage;
    krb5_boolean valid;
    krb5_boolean not_newer;

    assert(req_s4u_user != NULL);

    switch (subkey->enctype) {
    case ENCTYPE_DES_CBC_CRC:
    case ENCTYPE_DES_CBC_MD4:
    case ENCTYPE_DES_CBC_MD5:
    case ENCTYPE_DES3_CBC_SHA1:
    case ENCTYPE_DES3_CBC_RAW:
    case ENCTYPE_ARCFOUR_HMAC:
    case ENCTYPE_ARCFOUR_HMAC_EXP :
        not_newer = TRUE;
        break;
    default:
        not_newer = FALSE;
        break;
    }

    enc_s4u_padata = krb5int_find_pa_data(context,
                                          enc_padata,
                                          KRB5_PADATA_S4U_X509_USER);

     
    rep_s4u_padata = krb5int_find_pa_data(context,
                                          rep_padata,
                                          KRB5_PADATA_S4U_X509_USER);
    if (rep_s4u_padata == NULL) {
        if (not_newer == FALSE || enc_s4u_padata != NULL)
            return KRB5_KDCREP_MODIFIED;
        else
            return 0;
    }

    data.length = rep_s4u_padata->length;
    data.data = (char *)rep_s4u_padata->contents;

    code = decode_krb5_pa_s4u_x509_user(&data, &rep_s4u_user);
    if (code != 0)
        goto cleanup;

    if (rep_s4u_user->user_id.nonce != req_s4u_user->user_id.nonce) {
        code = KRB5_KDCREP_MODIFIED;
        goto cleanup;
    }

    code = encode_krb5_s4u_userid(&rep_s4u_user->user_id, &datap);
    if (code != 0)
        goto cleanup;

    if (rep_s4u_user->user_id.options & KRB5_S4U_OPTS_USE_REPLY_KEY_USAGE)
        usage = KRB5_KEYUSAGE_PA_S4U_X509_USER_REPLY;
    else
        usage = KRB5_KEYUSAGE_PA_S4U_X509_USER_REQUEST;

    code = krb5_c_verify_checksum(context, subkey, usage, datap,
                                  &rep_s4u_user->cksum, &valid);
    if (code != 0)
        goto cleanup;
    if (valid == FALSE) {
        code = KRB5_KDCREP_MODIFIED;
        goto cleanup;
    }

     
    if (not_newer) {
        if (enc_s4u_padata == NULL) {
            if (rep_s4u_user->user_id.options &
                KRB5_S4U_OPTS_USE_REPLY_KEY_USAGE) {
                code = KRB5_KDCREP_MODIFIED;
                goto cleanup;
            }
        } else {
            if (enc_s4u_padata->length !=
                req_s4u_user->cksum.length + rep_s4u_user->cksum.length) {
                code = KRB5_KDCREP_MODIFIED;
                goto cleanup;
            }
            if (memcmp(enc_s4u_padata->contents,
                       req_s4u_user->cksum.contents,
                       req_s4u_user->cksum.length) ||
                memcmp(&enc_s4u_padata->contents[req_s4u_user->cksum.length],
                       rep_s4u_user->cksum.contents,
                       rep_s4u_user->cksum.length)) {
                code = KRB5_KDCREP_MODIFIED;
                goto cleanup;
            }
        }
    } else if (!krb5_c_is_keyed_cksum(rep_s4u_user->cksum.checksum_type)) {
        code = KRB5KRB_AP_ERR_INAPP_CKSUM;
        goto cleanup;
    }

cleanup:
    krb5_free_pa_s4u_x509_user(context, rep_s4u_user);
    krb5_free_data(context, datap);

    return code;
}