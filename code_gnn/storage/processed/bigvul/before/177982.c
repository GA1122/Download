 int ASN1_item_ex_d2i(ASN1_VALUE **pval, const unsigned char **in, long len,
                      const ASN1_ITEM *it,
                      int tag, int aclass, char opt, ASN1_TLC *ctx)
 {
     const ASN1_TEMPLATE *tt, *errtt = NULL;
     const ASN1_COMPAT_FUNCS *cf;
     const ASN1_EXTERN_FUNCS *ef;
     const ASN1_AUX *aux = it->funcs;
     ASN1_aux_cb *asn1_cb;
     const unsigned char *p = NULL, *q;
     unsigned char *wp = NULL;    
     unsigned char imphack = 0, oclass;
     char seq_eoc, seq_nolen, cst, isopt;
     long tmplen;
     int i;
     int otag;
     int ret = 0;
     ASN1_VALUE **pchptr, *ptmpval;
     if (!pval)
         return 0;
     if (aux && aux->asn1_cb)
         asn1_cb = aux->asn1_cb;
     else
         asn1_cb = 0;
 
     switch (it->itype) {
     case ASN1_ITYPE_PRIMITIVE:
         if (it->templates) {
              
             if ((tag != -1) || opt) {
                 ASN1err(ASN1_F_ASN1_ITEM_EX_D2I,
                         ASN1_R_ILLEGAL_OPTIONS_ON_ITEM_TEMPLATE);
                 goto err;
             }
             return asn1_template_ex_d2i(pval, in, len,
                                         it->templates, opt, ctx);
         }
         return asn1_d2i_ex_primitive(pval, in, len, it,
                                      tag, aclass, opt, ctx);
         break;
 
     case ASN1_ITYPE_MSTRING:
         p = *in;
          
         ret = asn1_check_tlen(NULL, &otag, &oclass, NULL, NULL,
                               &p, len, -1, 0, 1, ctx);
         if (!ret) {
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
             goto err;
         }
 
          
         if (oclass != V_ASN1_UNIVERSAL) {
              
             if (opt)
                 return -1;
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_MSTRING_NOT_UNIVERSAL);
             goto err;
         }
          
         if (!(ASN1_tag2bit(otag) & it->utype)) {
              
             if (opt)
                 return -1;
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_MSTRING_WRONG_TAG);
             goto err;
         }
         return asn1_d2i_ex_primitive(pval, in, len, it, otag, 0, 0, ctx);
 
     case ASN1_ITYPE_EXTERN:
          
         ef = it->funcs;
         return ef->asn1_ex_d2i(pval, in, len, it, tag, aclass, opt, ctx);
 
     case ASN1_ITYPE_COMPAT:
          
         cf = it->funcs;
 
          
         if (opt) {
             int exptag;
             p = *in;
             if (tag == -1)
                 exptag = it->utype;
             else
                 exptag = tag;
              
 
             ret = asn1_check_tlen(NULL, NULL, NULL, NULL, NULL,
                                   &p, len, exptag, aclass, 1, ctx);
             if (!ret) {
                 ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
                 goto err;
             }
             if (ret == -1)
                 return -1;
         }
 
          
 
         if (tag != -1) {
             wp = *(unsigned char **)in;
             imphack = *wp;
             if (p == NULL) {
                 ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
                 goto err;
             }
             *wp = (unsigned char)((*p & V_ASN1_CONSTRUCTED)
                                   | it->utype);
         }
 
         ptmpval = cf->asn1_d2i(pval, in, len);
 
         if (tag != -1)
             *wp = imphack;
 
         if (ptmpval)
             return 1;
 
         ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
         goto err;
 
      case ASN1_ITYPE_CHOICE:
          if (asn1_cb && !asn1_cb(ASN1_OP_D2I_PRE, pval, it, NULL))
              goto auxerr;
         
        if (!*pval && !ASN1_item_ex_new(pval, it)) {
//         if (*pval) {
//              
//             i = asn1_get_choice_selector(pval, it);
//             if ((i >= 0) && (i < it->tcount)) {
//                 tt = it->templates + i;
//                 pchptr = asn1_get_field_ptr(pval, tt);
//                 ASN1_template_free(pchptr, tt);
//                 asn1_set_choice_selector(pval, -1, it);
//             }
//         } else if (!ASN1_item_ex_new(pval, it)) {
              ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
              goto err;
          }
             ret = asn1_template_ex_d2i(pchptr, &p, len, tt, 1, ctx);
              
             if (ret == -1)
                 continue;
              
             if (ret > 0)
                 break;
              
             errtt = tt;
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
             goto err;
         }
 
          
         if (i == it->tcount) {
              
             if (opt) {
                  
                 ASN1_item_ex_free(pval, it);
                 return -1;
             }
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_NO_MATCHING_CHOICE_TYPE);
             goto err;
         }
 
         asn1_set_choice_selector(pval, i, it);
         *in = p;
         if (asn1_cb && !asn1_cb(ASN1_OP_D2I_POST, pval, it, NULL))
             goto auxerr;
         return 1;
 
     case ASN1_ITYPE_NDEF_SEQUENCE:
     case ASN1_ITYPE_SEQUENCE:
         p = *in;
         tmplen = len;
 
          
         if (tag == -1) {
             tag = V_ASN1_SEQUENCE;
             aclass = V_ASN1_UNIVERSAL;
         }
          
         ret = asn1_check_tlen(&len, NULL, NULL, &seq_eoc, &cst,
                               &p, len, tag, aclass, opt, ctx);
         if (!ret) {
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
             goto err;
         } else if (ret == -1)
             return -1;
         if (aux && (aux->flags & ASN1_AFLG_BROKEN)) {
             len = tmplen - (p - *in);
             seq_nolen = 1;
         }
          
         else
             seq_nolen = seq_eoc;
         if (!cst) {
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_SEQUENCE_NOT_CONSTRUCTED);
             goto err;
         }
 
         if (!*pval && !ASN1_item_ex_new(pval, it)) {
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ERR_R_NESTED_ASN1_ERROR);
             goto err;
         }
 
         if (asn1_cb && !asn1_cb(ASN1_OP_D2I_PRE, pval, it, NULL))
             goto auxerr;
 
          
         for (i = 0, tt = it->templates; i < it->tcount; i++, tt++) {
             const ASN1_TEMPLATE *seqtt;
             ASN1_VALUE **pseqval;
          if (asn1_cb && !asn1_cb(ASN1_OP_D2I_PRE, pval, it, NULL))
              goto auxerr;
  
//          
//         for (i = 0, tt = it->templates; i < it->tcount; i++, tt++) {
//             if (tt->flags & ASN1_TFLG_ADB_MASK) {
//                 const ASN1_TEMPLATE *seqtt;
//                 ASN1_VALUE **pseqval;
//                 seqtt = asn1_do_adb(pval, tt, 1);
//                 pseqval = asn1_get_field_ptr(pval, seqtt);
//                 ASN1_template_free(pseqval, seqtt);
//             }
//         }
// 
           
          for (i = 0, tt = it->templates; i < it->tcount; i++, tt++) {
              const ASN1_TEMPLATE *seqtt;
             }
              
             if (i == (it->tcount - 1))
                 isopt = 0;
             else
                 isopt = (char)(seqtt->flags & ASN1_TFLG_OPTIONAL);
              
 
             ret = asn1_template_ex_d2i(pseqval, &p, len, seqtt, isopt, ctx);
             if (!ret) {
                 errtt = seqtt;
                 goto err;
             } else if (ret == -1) {
                  
                 ASN1_template_free(pseqval, seqtt);
                 continue;
             }
              
             len -= p - q;
         }
 
          
         if (seq_eoc && !asn1_check_eoc(&p, len)) {
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_MISSING_EOC);
             goto err;
         }
          
         if (!seq_nolen && len) {
             ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_SEQUENCE_LENGTH_MISMATCH);
             goto err;
         }
 
          
         for (; i < it->tcount; tt++, i++) {
             const ASN1_TEMPLATE *seqtt;
             seqtt = asn1_do_adb(pval, tt, 1);
             if (!seqtt)
                 goto err;
             if (seqtt->flags & ASN1_TFLG_OPTIONAL) {
                 ASN1_VALUE **pseqval;
                 pseqval = asn1_get_field_ptr(pval, seqtt);
                 ASN1_template_free(pseqval, seqtt);
             } else {
                 errtt = seqtt;
                 ASN1err(ASN1_F_ASN1_ITEM_EX_D2I, ASN1_R_FIELD_MISSING);
                 goto err;
             }
         }
          
         if (!asn1_enc_save(pval, *in, p - *in, it))
             goto auxerr;
         *in = p;
         if (asn1_cb && !asn1_cb(ASN1_OP_D2I_POST, pval, it, NULL))
             goto auxerr;
         return 1;
 
     default:
         return 0;
     }