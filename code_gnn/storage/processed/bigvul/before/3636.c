static RSA_OAEP_PARAMS *rsa_oaep_decode(const X509_ALGOR *alg,
                                        X509_ALGOR **pmaskHash)
{
    const unsigned char *p;
    int plen;
    RSA_OAEP_PARAMS *pss;

    *pmaskHash = NULL;

    if (!alg->parameter || alg->parameter->type != V_ASN1_SEQUENCE)
        return NULL;
    p = alg->parameter->value.sequence->data;
    plen = alg->parameter->value.sequence->length;
    pss = d2i_RSA_OAEP_PARAMS(NULL, &p, plen);

    if (!pss)
        return NULL;

    *pmaskHash = rsa_mgf1_decode(pss->maskGenFunc);

    return pss;
}
