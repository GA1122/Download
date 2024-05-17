static int asn1_find_end(const unsigned char **in, long len, char inf)
{
    int expected_eoc;
    long plen;
    const unsigned char *p = *in, *q;
     
    if (inf == 0) {
        *in += len;
        return 1;
    }
    expected_eoc = 1;
     
    while (len > 0) {
        if (asn1_check_eoc(&p, len)) {
            expected_eoc--;
            if (expected_eoc == 0)
                break;
            len -= 2;
            continue;
        }
        q = p;
         
        if (!asn1_check_tlen(&plen, NULL, NULL, &inf, NULL, &p, len,
                             -1, 0, 0, NULL)) {
            ASN1err(ASN1_F_ASN1_FIND_END, ERR_R_NESTED_ASN1_ERROR);
            return 0;
        }
        if (inf)
            expected_eoc++;
        else
            p += plen;
        len -= p - q;
    }
    if (expected_eoc) {
        ASN1err(ASN1_F_ASN1_FIND_END, ASN1_R_MISSING_EOC);
        return 0;
    }
    *in = p;
    return 1;
}
