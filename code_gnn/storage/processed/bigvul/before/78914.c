static int x509_v3_basic_constraints(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    int ret = X509_OK;
    int lenSeq = 0;

    if ((offset = asn1_is_basic_constraints(cert, offset)) == 0)
        goto end_contraints;

    x509_ctx->basic_constraint_present = true;
    x509_ctx->basic_constraint_is_critical = 
                    asn1_is_critical_ext(cert, &offset);

     
    x509_ctx->basic_constraint_cA = 0;
     
    x509_ctx->basic_constraint_pathLenConstraint = 10000;
    
    if ((asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) < 0) ||
            ((lenSeq = asn1_next_obj(cert, &offset, ASN1_SEQUENCE)) < 0))
    {
        ret = X509_NOT_OK;       
    }
    
     
    if ((lenSeq>0)&&(asn1_get_bool(cert, &offset, 
            &x509_ctx->basic_constraint_cA) < 0))
    {
        ret = X509_NOT_OK;
    }
    
     
    if ((lenSeq>3) && (asn1_get_int(cert, &offset, 
            &x509_ctx->basic_constraint_pathLenConstraint) < 0))
    {
        ret = X509_NOT_OK;
    }

end_contraints:
    return ret;
}
