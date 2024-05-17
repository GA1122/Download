static int asn1_template_ex_i2d(ASN1_VALUE **pval, unsigned char **out,
                                const ASN1_TEMPLATE *tt, int tag, int iclass)
{
    int i, ret, flags, ttag, tclass, ndef;
    flags = tt->flags;
     
    if (flags & ASN1_TFLG_TAG_MASK) {
         
        if (tag != -1)
             
            return -1;
         
        ttag = tt->tag;
        tclass = flags & ASN1_TFLG_TAG_CLASS;
    } else if (tag != -1) {
         
        ttag = tag;
        tclass = iclass & ASN1_TFLG_TAG_CLASS;
    } else {
        ttag = -1;
        tclass = 0;
    }
     
    iclass &= ~ASN1_TFLG_TAG_CLASS;

     

     
    if ((flags & ASN1_TFLG_NDEF) && (iclass & ASN1_TFLG_NDEF))
        ndef = 2;
    else
        ndef = 1;

    if (flags & ASN1_TFLG_SK_MASK) {
         
        STACK_OF(ASN1_VALUE) *sk = (STACK_OF(ASN1_VALUE) *)*pval;
        int isset, sktag, skaclass;
        int skcontlen, sklen;
        ASN1_VALUE *skitem;

        if (!*pval)
            return 0;

        if (flags & ASN1_TFLG_SET_OF) {
            isset = 1;
             
            if (flags & ASN1_TFLG_SEQUENCE_OF)
                isset = 2;
        } else
            isset = 0;

         
        if ((ttag != -1) && !(flags & ASN1_TFLG_EXPTAG)) {
            sktag = ttag;
            skaclass = tclass;
        } else {
            skaclass = V_ASN1_UNIVERSAL;
            if (isset)
                sktag = V_ASN1_SET;
            else
                sktag = V_ASN1_SEQUENCE;
        }

         
        skcontlen = 0;
        for (i = 0; i < sk_ASN1_VALUE_num(sk); i++) {
            skitem = sk_ASN1_VALUE_value(sk, i);
            skcontlen += ASN1_item_ex_i2d(&skitem, NULL,
                                          ASN1_ITEM_ptr(tt->item),
                                          -1, iclass);
        }
        sklen = ASN1_object_size(ndef, skcontlen, sktag);
         
        if (flags & ASN1_TFLG_EXPTAG)
            ret = ASN1_object_size(ndef, sklen, ttag);
        else
            ret = sklen;

        if (!out)
            return ret;

         
         
        if (flags & ASN1_TFLG_EXPTAG)
            ASN1_put_object(out, ndef, sklen, ttag, tclass);
         
        ASN1_put_object(out, ndef, skcontlen, sktag, skaclass);
         
        asn1_set_seq_out(sk, out, skcontlen, ASN1_ITEM_ptr(tt->item),
                         isset, iclass);
        if (ndef == 2) {
            ASN1_put_eoc(out);
            if (flags & ASN1_TFLG_EXPTAG)
                ASN1_put_eoc(out);
        }

        return ret;
    }

    if (flags & ASN1_TFLG_EXPTAG) {
         
         
        i = ASN1_item_ex_i2d(pval, NULL, ASN1_ITEM_ptr(tt->item), -1, iclass);
        if (!i)
            return 0;
         
        ret = ASN1_object_size(ndef, i, ttag);
        if (out) {
             
            ASN1_put_object(out, ndef, i, ttag, tclass);
            ASN1_item_ex_i2d(pval, out, ASN1_ITEM_ptr(tt->item), -1, iclass);
            if (ndef == 2)
                ASN1_put_eoc(out);
        }
        return ret;
    }

     
    return ASN1_item_ex_i2d(pval, out, ASN1_ITEM_ptr(tt->item),
                            ttag, tclass | iclass);

}
