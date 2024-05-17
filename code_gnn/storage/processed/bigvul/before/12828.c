BIGNUM *ASN1_INTEGER_to_BN(const ASN1_INTEGER *ai, BIGNUM *bn)
{
    BIGNUM *ret;

    if ((ret = BN_bin2bn(ai->data, ai->length, bn)) == NULL)
        ASN1err(ASN1_F_ASN1_INTEGER_TO_BN, ASN1_R_BN_LIB);
    else if (ai->type == V_ASN1_NEG_INTEGER)
        BN_set_negative(ret, 1);
    return (ret);
}
