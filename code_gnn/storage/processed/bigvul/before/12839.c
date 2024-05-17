int ASN1_item_ex_i2d(ASN1_VALUE **pval, unsigned char **out,
                     const ASN1_ITEM *it, int tag, int aclass)
{
    const ASN1_TEMPLATE *tt = NULL;
    unsigned char *p = NULL;
    int i, seqcontlen, seqlen, ndef = 1;
    const ASN1_COMPAT_FUNCS *cf;
    const ASN1_EXTERN_FUNCS *ef;
    const ASN1_AUX *aux = it->funcs;
    ASN1_aux_cb *asn1_cb = 0;

    if ((it->itype != ASN1_ITYPE_PRIMITIVE) && !*pval)
        return 0;

    if (aux && aux->asn1_cb)
        asn1_cb = aux->asn1_cb;

    switch (it->itype) {

    case ASN1_ITYPE_PRIMITIVE:
        if (it->templates)
            return asn1_template_ex_i2d(pval, out, it->templates,
                                        tag, aclass);
        return asn1_i2d_ex_primitive(pval, out, it, tag, aclass);
        break;

    case ASN1_ITYPE_MSTRING:
        return asn1_i2d_ex_primitive(pval, out, it, -1, aclass);

    case ASN1_ITYPE_CHOICE:
        if (asn1_cb && !asn1_cb(ASN1_OP_I2D_PRE, pval, it, NULL))
            return 0;
        i = asn1_get_choice_selector(pval, it);
        if ((i >= 0) && (i < it->tcount)) {
            ASN1_VALUE **pchval;
            const ASN1_TEMPLATE *chtt;
            chtt = it->templates + i;
            pchval = asn1_get_field_ptr(pval, chtt);
            return asn1_template_ex_i2d(pchval, out, chtt, -1, aclass);
        }
         
        if (asn1_cb && !asn1_cb(ASN1_OP_I2D_POST, pval, it, NULL))
            return 0;
        break;

    case ASN1_ITYPE_EXTERN:
         
        ef = it->funcs;
        return ef->asn1_ex_i2d(pval, out, it, tag, aclass);

    case ASN1_ITYPE_COMPAT:
         
        cf = it->funcs;
        if (out)
            p = *out;
        i = cf->asn1_i2d(*pval, out);
         
        if (out && (tag != -1))
            *p = aclass | tag | (*p & V_ASN1_CONSTRUCTED);
        return i;

    case ASN1_ITYPE_NDEF_SEQUENCE:
         
        if (aclass & ASN1_TFLG_NDEF)
            ndef = 2;
         

    case ASN1_ITYPE_SEQUENCE:
        i = asn1_enc_restore(&seqcontlen, out, pval, it);
         
        if (i < 0)
            return 0;
         
        if (i > 0)
            return seqcontlen;
         
        seqcontlen = 0;
         
        if (tag == -1) {
            tag = V_ASN1_SEQUENCE;
             
            aclass = (aclass & ~ASN1_TFLG_TAG_CLASS)
                | V_ASN1_UNIVERSAL;
        }
        if (asn1_cb && !asn1_cb(ASN1_OP_I2D_PRE, pval, it, NULL))
            return 0;
         
        for (i = 0, tt = it->templates; i < it->tcount; tt++, i++) {
            const ASN1_TEMPLATE *seqtt;
            ASN1_VALUE **pseqval;
            seqtt = asn1_do_adb(pval, tt, 1);
            if (!seqtt)
                return 0;
            pseqval = asn1_get_field_ptr(pval, seqtt);
             
            seqcontlen += asn1_template_ex_i2d(pseqval, NULL, seqtt,
                                               -1, aclass);
        }

        seqlen = ASN1_object_size(ndef, seqcontlen, tag);
        if (!out)
            return seqlen;
         
        ASN1_put_object(out, ndef, seqcontlen, tag, aclass);
        for (i = 0, tt = it->templates; i < it->tcount; tt++, i++) {
            const ASN1_TEMPLATE *seqtt;
            ASN1_VALUE **pseqval;
            seqtt = asn1_do_adb(pval, tt, 1);
            if (!seqtt)
                return 0;
            pseqval = asn1_get_field_ptr(pval, seqtt);
             
            asn1_template_ex_i2d(pseqval, out, seqtt, -1, aclass);
        }
        if (ndef == 2)
            ASN1_put_eoc(out);
        if (asn1_cb && !asn1_cb(ASN1_OP_I2D_POST, pval, it, NULL))
            return 0;
        return seqlen;

    default:
        return 0;

    }
    return 0;
}
