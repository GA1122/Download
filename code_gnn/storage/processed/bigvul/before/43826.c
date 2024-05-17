nonce_verify(krb5_context ctx, krb5_keyblock *armor_key,
             const krb5_data *nonce)
{
    krb5_error_code retval;
    krb5_timestamp ts;
    krb5_data *er = NULL;

    if (armor_key == NULL || nonce->data == NULL) {
        retval = EINVAL;
        goto out;
    }

     
    retval = decode_krb5_pa_otp_enc_req(nonce, &er);
    if (retval != 0)
        goto out;

     
    if (er->length != armor_key->length + sizeof(krb5_timestamp))
        goto out;

     
    ts = load_32_be(er->data);
    retval = krb5_check_clockskew(ctx, ts);

out:
    krb5_free_data(ctx, er);
    return retval;
}
