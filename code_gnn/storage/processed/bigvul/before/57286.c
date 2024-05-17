gss_name_to_string(gss_name_t gss_name, gss_buffer_desc *str)
{
    OM_uint32 status, minor_stat;
    gss_OID gss_type;
    const char pref[] = KRB5_WELLKNOWN_NAMESTR "/" KRB5_ANONYMOUS_PRINCSTR "@";
    const size_t preflen = sizeof(pref) - 1;

    status = gss_display_name(&minor_stat, gss_name, str, &gss_type);
    if (status != GSS_S_COMPLETE)
        return 1;
    if (gss_oid_equal(gss_type, GSS_C_NT_ANONYMOUS)) {
         
        if (str->length < preflen || memcmp(str->value, pref, preflen) != 0)
            return 1;
    } else if (!gss_oid_equal(gss_type, GSS_KRB5_NT_PRINCIPAL_NAME)) {
        return 1;
    }
    return 0;
}