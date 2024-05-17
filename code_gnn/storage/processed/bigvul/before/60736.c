get_key_cb(char *buf, int size, int rwflag, void *userdata)
{
    struct get_key_cb_data *data = userdata;
    pkinit_identity_crypto_context id_cryptoctx;
    krb5_data rdat;
    krb5_prompt kprompt;
    krb5_prompt_type prompt_type;
    krb5_error_code retval;
    char *prompt;

    if (data->id_cryptoctx->defer_id_prompt) {
         
        pkinit_set_deferred_id(&data->id_cryptoctx->deferred_ids,
                               data->fsname, 0, NULL);
        return -1;
    }
    if (data->password == NULL) {
         
        if (data->id_cryptoctx->prompter == NULL)
            return -1;
        if (asprintf(&prompt, "%s %s", _("Pass phrase for"),
                     data->filename) < 0)
            return -1;
        rdat.data = buf;
        rdat.length = size;
        kprompt.prompt = prompt;
        kprompt.hidden = 1;
        kprompt.reply = &rdat;
        prompt_type = KRB5_PROMPT_TYPE_PREAUTH;

         
        k5int_set_prompt_types(data->context, &prompt_type);
        id_cryptoctx = data->id_cryptoctx;
        retval = (data->id_cryptoctx->prompter)(data->context,
                                                id_cryptoctx->prompter_data,
                                                NULL, NULL, 1, &kprompt);
        k5int_set_prompt_types(data->context, 0);
        free(prompt);
        if (retval != 0)
            return -1;
    } else {
         
        rdat.length = strlen(data->password);
        if ((int)rdat.length >= size)
            return -1;
        snprintf(buf, size, "%s", data->password);
    }
    return (int)rdat.length;
}
