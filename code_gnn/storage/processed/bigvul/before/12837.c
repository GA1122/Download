static int asn1_template_noexp_d2i(ASN1_VALUE **val,
                                   const unsigned char **in, long len,
                                   const ASN1_TEMPLATE *tt, char opt,
                                   ASN1_TLC *ctx)
{
    int flags, aclass;
    int ret;
    const unsigned char *p, *q;
    if (!val)
        return 0;
    flags = tt->flags;
    aclass = flags & ASN1_TFLG_TAG_CLASS;

    p = *in;
    q = p;

    if (flags & ASN1_TFLG_SK_MASK) {
         
        int sktag, skaclass;
        char sk_eoc;
         
        if (flags & ASN1_TFLG_IMPTAG) {
            sktag = tt->tag;
            skaclass = aclass;
        } else {
            skaclass = V_ASN1_UNIVERSAL;
            if (flags & ASN1_TFLG_SET_OF)
                sktag = V_ASN1_SET;
            else
                sktag = V_ASN1_SEQUENCE;
        }
         
        ret = asn1_check_tlen(&len, NULL, NULL, &sk_eoc, NULL,
                              &p, len, sktag, skaclass, opt, ctx);
        if (!ret) {
            ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I, ERR_R_NESTED_ASN1_ERROR);
            return 0;
        } else if (ret == -1)
            return -1;
        if (!*val)
            *val = (ASN1_VALUE *)sk_new_null();
        else {
             
            STACK_OF(ASN1_VALUE) *sktmp = (STACK_OF(ASN1_VALUE) *)*val;
            ASN1_VALUE *vtmp;
            while (sk_ASN1_VALUE_num(sktmp) > 0) {
                vtmp = sk_ASN1_VALUE_pop(sktmp);
                ASN1_item_ex_free(&vtmp, ASN1_ITEM_ptr(tt->item));
            }
        }

        if (!*val) {
            ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I, ERR_R_MALLOC_FAILURE);
            goto err;
        }

         
        while (len > 0) {
            ASN1_VALUE *skfield;
            q = p;
             
            if (asn1_check_eoc(&p, len)) {
                if (!sk_eoc) {
                    ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I,
                            ASN1_R_UNEXPECTED_EOC);
                    goto err;
                }
                len -= p - q;
                sk_eoc = 0;
                break;
            }
            skfield = NULL;
            if (!ASN1_item_ex_d2i(&skfield, &p, len,
                                  ASN1_ITEM_ptr(tt->item), -1, 0, 0, ctx)) {
                ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I,
                        ERR_R_NESTED_ASN1_ERROR);
                goto err;
            }
            len -= p - q;
            if (!sk_ASN1_VALUE_push((STACK_OF(ASN1_VALUE) *)*val, skfield)) {
                ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I, ERR_R_MALLOC_FAILURE);
                goto err;
            }
        }
        if (sk_eoc) {
            ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I, ASN1_R_MISSING_EOC);
            goto err;
        }
    } else if (flags & ASN1_TFLG_IMPTAG) {
         
        ret = ASN1_item_ex_d2i(val, &p, len,
                               ASN1_ITEM_ptr(tt->item), tt->tag, aclass, opt,
                               ctx);
        if (!ret) {
            ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I, ERR_R_NESTED_ASN1_ERROR);
            goto err;
        } else if (ret == -1)
            return -1;
    } else {
         
        ret = ASN1_item_ex_d2i(val, &p, len, ASN1_ITEM_ptr(tt->item),
                               -1, tt->flags & ASN1_TFLG_COMBINE, opt, ctx);
        if (!ret) {
            ASN1err(ASN1_F_ASN1_TEMPLATE_NOEXP_D2I, ERR_R_NESTED_ASN1_ERROR);
            goto err;
        } else if (ret == -1)
            return -1;
    }

    *in = p;
    return 1;

 err:
    ASN1_template_free(val, tt);
    return 0;
}