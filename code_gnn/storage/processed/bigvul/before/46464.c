make_seal_token_v1 (krb5_context context,
                    krb5_key enc,
                    krb5_key seq,
                    uint64_t *seqnum,
                    int direction,
                    gss_buffer_t text,
                    gss_buffer_t token,
                    int signalg,
                    size_t cksum_size,
                    int sealalg,
                    int do_encrypt,
                    int toktype,
                    gss_OID oid)
{
    krb5_error_code code;
    size_t sumlen;
    char *data_ptr;
    krb5_data plaind;
    krb5_checksum md5cksum;
    krb5_checksum cksum;
     
    unsigned int conflen=0, tmsglen, tlen, msglen;
    unsigned char *t, *ptr;
    unsigned char *plain;
    unsigned char pad;
    krb5_keyusage sign_usage = KG_USAGE_SIGN;


    assert((!do_encrypt) || (toktype == KG_TOK_SEAL_MSG));
     
     
    if (do_encrypt || toktype == KG_TOK_SEAL_MSG)
        conflen = kg_confounder_size(context, enc->keyblock.enctype);
    else conflen = 0;

    if (toktype == KG_TOK_SEAL_MSG) {
        switch (sealalg) {
        case SEAL_ALG_MICROSOFT_RC4:
            msglen = conflen + text->length+1;
            pad = 1;
            break;
        default:
             
            msglen = (conflen+text->length+8)&(~7);
            pad = 8-(text->length%8);
        }
        tmsglen = msglen;
    } else {
        tmsglen = 0;
        msglen = text->length;
        pad = 0;
    }
    tlen = g_token_size((gss_OID) oid, 14+cksum_size+tmsglen);

    if ((t = (unsigned char *) gssalloc_malloc(tlen)) == NULL)
        return(ENOMEM);

     

    ptr = t;
    g_make_token_header(oid, 14+cksum_size+tmsglen, &ptr, toktype);

     
    store_16_le(signalg, &ptr[0]);

     
    if ((toktype == KG_TOK_SEAL_MSG) && do_encrypt) {
        store_16_le(sealalg, &ptr[2]);
    } else {
         
        ptr[2] = 0xff;
        ptr[3] = 0xff;
    }

     
    ptr[4] = 0xff;
    ptr[5] = 0xff;

     

     
    switch (signalg) {
    case SGN_ALG_DES_MAC_MD5:
    case SGN_ALG_MD2_5:
        md5cksum.checksum_type = CKSUMTYPE_RSA_MD5;
        break;
    case SGN_ALG_HMAC_SHA1_DES3_KD:
        md5cksum.checksum_type = CKSUMTYPE_HMAC_SHA1_DES3;
        break;
    case SGN_ALG_HMAC_MD5:
        md5cksum.checksum_type = CKSUMTYPE_HMAC_MD5_ARCFOUR;
        if (toktype != KG_TOK_SEAL_MSG)
            sign_usage = 15;
        break;
    default:
    case SGN_ALG_DES_MAC:
        abort ();
    }

    code = krb5_c_checksum_length(context, md5cksum.checksum_type, &sumlen);
    if (code) {
        gssalloc_free(t);
        return(code);
    }
    md5cksum.length = sumlen;


    if ((plain = (unsigned char *) xmalloc(msglen ? msglen : 1)) == NULL) {
        gssalloc_free(t);
        return(ENOMEM);
    }

    if (conflen) {
        if ((code = kg_make_confounder(context, enc->keyblock.enctype,
                                       plain))) {
            xfree(plain);
            gssalloc_free(t);
            return(code);
        }
    }

    memcpy(plain+conflen, text->value, text->length);
    if (pad) memset(plain+conflen+text->length, pad, pad);

     

     
    if (! (data_ptr = xmalloc(8 + msglen))) {
        xfree(plain);
        gssalloc_free(t);
        return(ENOMEM);
    }
    (void) memcpy(data_ptr, ptr-2, 8);
    (void) memcpy(data_ptr+8, plain, msglen);
    plaind.length = 8 + msglen;
    plaind.data = data_ptr;
    code = krb5_k_make_checksum(context, md5cksum.checksum_type, seq,
                                sign_usage, &plaind, &md5cksum);
    xfree(data_ptr);

    if (code) {
        xfree(plain);
        gssalloc_free(t);
        return(code);
    }
    switch(signalg) {
    case SGN_ALG_DES_MAC_MD5:
    case 3:

        code = kg_encrypt_inplace(context, seq, KG_USAGE_SEAL,
                                  (g_OID_equal(oid, gss_mech_krb5_old) ?
                                   seq->keyblock.contents : NULL),
                                  md5cksum.contents, 16);
        if (code) {
            krb5_free_checksum_contents(context, &md5cksum);
            xfree (plain);
            gssalloc_free(t);
            return code;
        }

        cksum.length = cksum_size;
        cksum.contents = md5cksum.contents + 16 - cksum.length;

        memcpy(ptr+14, cksum.contents, cksum.length);
        break;

    case SGN_ALG_HMAC_SHA1_DES3_KD:
         
        if (md5cksum.length != cksum_size)
            abort ();
        memcpy (ptr+14, md5cksum.contents, md5cksum.length);
        break;
    case SGN_ALG_HMAC_MD5:
        memcpy (ptr+14, md5cksum.contents, cksum_size);
        break;
    }

    krb5_free_checksum_contents(context, &md5cksum);

     

    if ((code = kg_make_seq_num(context, seq, direction?0:0xff,
                                (krb5_ui_4)*seqnum, ptr+14, ptr+6))) {
        xfree (plain);
        gssalloc_free(t);
        return(code);
    }

    if (do_encrypt) {
        switch(sealalg) {
        case SEAL_ALG_MICROSOFT_RC4:
        {
            unsigned char bigend_seqnum[4];
            krb5_keyblock *enc_key;
            int i;
            store_32_be(*seqnum, bigend_seqnum);
            code = krb5_k_key_keyblock(context, enc, &enc_key);
            if (code)
            {
                xfree(plain);
                gssalloc_free(t);
                return(code);
            }
            assert (enc_key->length == 16);
            for (i = 0; i <= 15; i++)
                ((char *) enc_key->contents)[i] ^=0xf0;
            code = kg_arcfour_docrypt (enc_key, 0,
                                       bigend_seqnum, 4,
                                       plain, tmsglen,
                                       ptr+14+cksum_size);
            krb5_free_keyblock (context, enc_key);
            if (code)
            {
                xfree(plain);
                gssalloc_free(t);
                return(code);
            }
        }
        break;
        default:
            if ((code = kg_encrypt(context, enc, KG_USAGE_SEAL, NULL,
                                   (krb5_pointer) plain,
                                   (krb5_pointer) (ptr+cksum_size+14),
                                   tmsglen))) {
                xfree(plain);
                gssalloc_free(t);
                return(code);
            }
        }
    }else {
        if (tmsglen)
            memcpy(ptr+14+cksum_size, plain, tmsglen);
    }
    xfree(plain);


     

    (*seqnum)++;
    *seqnum &= 0xffffffffL;

    token->length = tlen;
    token->value = (void *) t;

    return(0);
}
