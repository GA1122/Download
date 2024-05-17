certauth_pkinit_san_initvt(krb5_context context, int maj_ver, int min_ver,
                           krb5_plugin_vtable vtable)
{
    krb5_certauth_vtable vt;

    if (maj_ver != 1)
        return KRB5_PLUGIN_VER_NOTSUPP;
    vt = (krb5_certauth_vtable)vtable;
    vt->name = "pkinit_san";
    vt->authorize = pkinit_san_authorize;
    return 0;
}
