static int asn1_i2d_ex_primitive(ASN1_VALUE **pval, unsigned char **out,
                                 const ASN1_ITEM *it, int tag, int aclass)
{
    int len;
    int utype;
    int usetag;
    int ndef = 0;

    utype = it->utype;

     

    len = asn1_ex_i2c(pval, NULL, &utype, it);

     
    if ((utype == V_ASN1_SEQUENCE) || (utype == V_ASN1_SET) ||
        (utype == V_ASN1_OTHER))
        usetag = 0;
    else
        usetag = 1;

     

    if (len == -1)
        return 0;

     
    if (len == -2) {
        ndef = 2;
        len = 0;
    }

     
    if (tag == -1)
        tag = utype;

     
    if (out) {
        if (usetag)
            ASN1_put_object(out, ndef, len, tag, aclass);
        asn1_ex_i2c(pval, *out, &utype, it);
        if (ndef)
            ASN1_put_eoc(out);
        else
            *out += len;
    }

    if (usetag)
        return ASN1_object_size(ndef, len, tag);
    return len;
}
