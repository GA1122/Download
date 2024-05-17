kdc_check_transited_list(kdc_realm_t *kdc_active_realm,
                         const krb5_data *trans,
                         const krb5_data *realm1,
                         const krb5_data *realm2)
{
    krb5_error_code             code;

     
    code = krb5_db_check_transited_realms(kdc_context, trans, realm1, realm2);
    if (code != KRB5_PLUGIN_OP_NOTSUPP && code != KRB5_PLUGIN_NO_HANDLE)
        return code;

     
    return krb5_check_transited_list(kdc_context, trans, realm1, realm2);
}
