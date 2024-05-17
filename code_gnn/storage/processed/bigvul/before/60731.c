create_contentinfo(krb5_context context, ASN1_OBJECT *oid,
                   unsigned char *data, size_t data_len, PKCS7 **p7_out)
{
    PKCS7 *p7 = NULL;
    ASN1_OCTET_STRING *ostr = NULL;

    *p7_out = NULL;

    ostr = ASN1_OCTET_STRING_new();
    if (ostr == NULL)
        goto oom;
    if (!ASN1_OCTET_STRING_set(ostr, (unsigned char *)data, data_len))
        goto oom;

    p7 = PKCS7_new();
    if (p7 == NULL)
        goto oom;
    p7->type = OBJ_dup(oid);
    if (p7->type == NULL)
        goto oom;

    if (OBJ_obj2nid(oid) == NID_pkcs7_data) {
         
        p7->d.data = ostr;
    } else {
        p7->d.other = ASN1_TYPE_new();
        if (p7->d.other == NULL)
            goto oom;
        p7->d.other->type = V_ASN1_OCTET_STRING;
        p7->d.other->value.octet_string = ostr;
    }

    *p7_out = p7;
    return 0;

oom:
    if (ostr != NULL)
        ASN1_OCTET_STRING_free(ostr);
    if (p7 != NULL)
        PKCS7_free(p7);
    return ENOMEM;
}