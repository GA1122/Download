pkinit_server_plugin_init(krb5_context context,
                          krb5_kdcpreauth_moddata *moddata_out,
                          const char **realmnames)
{
    krb5_error_code retval = ENOMEM;
    pkinit_kdc_context plgctx, *realm_contexts = NULL;
    certauth_handle *certauth_modules = NULL;
    krb5_kdcpreauth_moddata moddata;
    size_t  i, j;
    size_t numrealms;

    retval = pkinit_accessor_init();
    if (retval)
        return retval;

     
    for (i = 0; realmnames[i] != NULL; i++) {};
    numrealms = i;

    realm_contexts = calloc(numrealms+1, sizeof(pkinit_kdc_context));
    if (realm_contexts == NULL)
        return ENOMEM;

    for (i = 0, j = 0; i < numrealms; i++) {
        TRACE_PKINIT_SERVER_INIT_REALM(context, realmnames[i]);
        retval = pkinit_server_plugin_init_realm(context, realmnames[i], &plgctx);
        if (retval == 0 && plgctx != NULL)
            realm_contexts[j++] = plgctx;
    }

    if (j == 0) {
        retval = EINVAL;
        krb5_set_error_message(context, retval,
                               _("No realms configured correctly for pkinit "
                                 "support"));
        goto errout;
    }

    retval = load_certauth_plugins(context, &certauth_modules);
    if (retval)
        goto errout;

    moddata = k5calloc(1, sizeof(*moddata), &retval);
    if (moddata == NULL)
        goto errout;
    moddata->realm_contexts = realm_contexts;
    moddata->certauth_modules = certauth_modules;
    *moddata_out = moddata;
    pkiDebug("%s: returning context at %p\n", __FUNCTION__, moddata);
    return 0;

errout:
    free_realm_contexts(context, realm_contexts);
    free_certauth_handles(context, certauth_modules);
    return retval;
}
