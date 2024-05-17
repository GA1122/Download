SSL_CIPHER *ssl3_choose_cipher(SSL *s, STACK_OF(SSL_CIPHER) *clnt,
                               STACK_OF(SSL_CIPHER) *srvr)
{
    SSL_CIPHER *c, *ret = NULL;
    STACK_OF(SSL_CIPHER) *prio, *allow;
    int i, ii, ok;
    CERT *cert;
    unsigned long alg_k, alg_a, mask_k, mask_a, emask_k, emask_a;

     
    cert = s->cert;

#if 0
     
    sk_SSL_CIPHER_set_cmp_func(srvr, ssl_cipher_ptr_id_cmp);
    sk_SSL_CIPHER_set_cmp_func(clnt, ssl_cipher_ptr_id_cmp);
#endif

#ifdef CIPHER_DEBUG
    fprintf(stderr, "Server has %d from %p:\n", sk_SSL_CIPHER_num(srvr),
            (void *)srvr);
    for (i = 0; i < sk_SSL_CIPHER_num(srvr); ++i) {
        c = sk_SSL_CIPHER_value(srvr, i);
        fprintf(stderr, "%p:%s\n", (void *)c, c->name);
    }
    fprintf(stderr, "Client sent %d from %p:\n", sk_SSL_CIPHER_num(clnt),
            (void *)clnt);
    for (i = 0; i < sk_SSL_CIPHER_num(clnt); ++i) {
        c = sk_SSL_CIPHER_value(clnt, i);
        fprintf(stderr, "%p:%s\n", (void *)c, c->name);
    }
#endif

    if (s->options & SSL_OP_CIPHER_SERVER_PREFERENCE || tls1_suiteb(s)) {
        prio = srvr;
        allow = clnt;
    } else {
        prio = clnt;
        allow = srvr;
    }

    tls1_set_cert_validity(s);

    for (i = 0; i < sk_SSL_CIPHER_num(prio); i++) {
        c = sk_SSL_CIPHER_value(prio, i);

         
        if ((c->algorithm_ssl & SSL_TLSV1_2) && !SSL_USE_TLS1_2_CIPHERS(s))
            continue;

        ssl_set_cert_masks(cert, c);
        mask_k = cert->mask_k;
        mask_a = cert->mask_a;
        emask_k = cert->export_mask_k;
        emask_a = cert->export_mask_a;
#ifndef OPENSSL_NO_SRP
        if (s->srp_ctx.srp_Mask & SSL_kSRP) {
            mask_k |= SSL_kSRP;
            emask_k |= SSL_kSRP;
            mask_a |= SSL_aSRP;
            emask_a |= SSL_aSRP;
        }
#endif

#ifdef KSSL_DEBUG
         
#endif                           

        alg_k = c->algorithm_mkey;
        alg_a = c->algorithm_auth;

#ifndef OPENSSL_NO_KRB5
        if (alg_k & SSL_kKRB5) {
            if (!kssl_keytab_is_available(s->kssl_ctx))
                continue;
        }
#endif                           
#ifndef OPENSSL_NO_PSK
         
        if ((alg_k & SSL_kPSK) && s->psk_server_callback == NULL)
            continue;
#endif                           

        if (SSL_C_IS_EXPORT(c)) {
            ok = (alg_k & emask_k) && (alg_a & emask_a);
#ifdef CIPHER_DEBUG
            fprintf(stderr, "%d:[%08lX:%08lX:%08lX:%08lX]%p:%s (export)\n",
                    ok, alg_k, alg_a, emask_k, emask_a, (void *)c, c->name);
#endif
        } else {
            ok = (alg_k & mask_k) && (alg_a & mask_a);
#ifdef CIPHER_DEBUG
            fprintf(stderr, "%d:[%08lX:%08lX:%08lX:%08lX]%p:%s\n", ok, alg_k,
                    alg_a, mask_k, mask_a, (void *)c, c->name);
#endif
        }

#ifndef OPENSSL_NO_TLSEXT
# ifndef OPENSSL_NO_EC
#  ifndef OPENSSL_NO_ECDH
         
        if (alg_k & SSL_kEECDH)
            ok = ok && tls1_check_ec_tmp_key(s, c->id);
#  endif                         
# endif                          
#endif                           

        if (!ok)
            continue;
        ii = sk_SSL_CIPHER_find(allow, c);
        if (ii >= 0) {
#if !defined(OPENSSL_NO_EC) && !defined(OPENSSL_NO_TLSEXT)
            if ((alg_k & SSL_kEECDH) && (alg_a & SSL_aECDSA)
                && s->s3->is_probably_safari) {
                if (!ret)
                    ret = sk_SSL_CIPHER_value(allow, ii);
                continue;
            }
#endif
            ret = sk_SSL_CIPHER_value(allow, ii);
            break;
        }
    }
    return (ret);
}
