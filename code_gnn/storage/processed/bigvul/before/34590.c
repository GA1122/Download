pkinit_pick_kdf_alg(krb5_context context,
                    krb5_octet_data **kdf_list,
                    krb5_octet_data **alg_oid)
{
    krb5_error_code retval = 0;
    krb5_octet_data *req_oid = NULL;
    const krb5_octet_data *supp_oid = NULL;
    krb5_octet_data *tmp_oid = NULL;
    int i, j = 0;

     
    *alg_oid = NULL;

     
    for (i = 0; NULL != (supp_oid = supported_kdf_alg_ids[i]); i++) {
         
        for (j = 0; NULL != (req_oid = kdf_list[j]); j++) {
            if ((req_oid->length == supp_oid->length) &&
                (0 == memcmp(req_oid->data, supp_oid->data, req_oid->length))) {
                tmp_oid = k5alloc(sizeof(krb5_octet_data), &retval);
                if (retval)
                    goto cleanup;
                tmp_oid->data = k5alloc(supp_oid->length, &retval);
                if (retval)
                    goto cleanup;
                tmp_oid->length = supp_oid->length;
                memcpy(tmp_oid->data, supp_oid->data, tmp_oid->length);
                *alg_oid = tmp_oid;
                 
                tmp_oid = NULL;
                goto cleanup;
            }
        }
    }
cleanup:
    if (tmp_oid)
        krb5_free_octet_data(context, tmp_oid);
    return retval;
}
