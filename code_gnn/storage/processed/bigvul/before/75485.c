krb5_get_as_key_noop(
    krb5_context context,
    krb5_principal client,
    krb5_enctype etype,
    krb5_prompter_fct prompter,
    void *prompter_data,
    krb5_data *salt,
    krb5_data *params,
    krb5_keyblock *as_key,
    void *gak_data,
    k5_response_items *ritems)
{
     
    return KRB5_PREAUTH_FAILED;
}