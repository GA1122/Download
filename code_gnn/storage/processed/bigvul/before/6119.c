static int request_certificate(SSL *s)
{
    const unsigned char *cp;
    unsigned char *p, *p2, *buf2;
    unsigned char *ccd;
    int i, j, ctype, ret = -1;
    unsigned long len;
    X509 *x509 = NULL;
    STACK_OF(X509) *sk = NULL;

    ccd = s->s2->tmp.ccl;
    if (s->state == SSL2_ST_SEND_REQUEST_CERTIFICATE_A) {
        p = (unsigned char *)s->init_buf->data;
        *(p++) = SSL2_MT_REQUEST_CERTIFICATE;
        *(p++) = SSL2_AT_MD5_WITH_RSA_ENCRYPTION;
        if (RAND_pseudo_bytes(ccd, SSL2_MIN_CERT_CHALLENGE_LENGTH) <= 0)
            return -1;
        memcpy(p, ccd, SSL2_MIN_CERT_CHALLENGE_LENGTH);

        s->state = SSL2_ST_SEND_REQUEST_CERTIFICATE_B;
        s->init_num = SSL2_MIN_CERT_CHALLENGE_LENGTH + 2;
        s->init_off = 0;
    }

    if (s->state == SSL2_ST_SEND_REQUEST_CERTIFICATE_B) {
        i = ssl2_do_write(s);
        if (i <= 0) {
            ret = i;
            goto end;
        }

        s->init_num = 0;
        s->state = SSL2_ST_SEND_REQUEST_CERTIFICATE_C;
    }

    if (s->state == SSL2_ST_SEND_REQUEST_CERTIFICATE_C) {
        p = (unsigned char *)s->init_buf->data;
         
        i = ssl2_read(s, (char *)&(p[s->init_num]), 6 - s->init_num);
         
        if (i < 3 - s->init_num) {
            ret = ssl2_part_read(s, SSL_F_REQUEST_CERTIFICATE, i);
            goto end;
        }
        s->init_num += i;

        if ((s->init_num >= 3) && (p[0] == SSL2_MT_ERROR)) {
            n2s(p, i);
            if (i != SSL2_PE_NO_CERTIFICATE) {
                 
                s->init_num -= 3;
                ret = ssl2_part_read(s, SSL_F_REQUEST_CERTIFICATE, 3);
                goto end;
            }

            if (s->msg_callback) {
                 
                s->msg_callback(0, s->version, 0, p, 3, s,
                                s->msg_callback_arg);
            }

             

            if (s->verify_mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT) {
                ssl2_return_error(s, SSL2_PE_BAD_CERTIFICATE);
                SSLerr(SSL_F_REQUEST_CERTIFICATE,
                       SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE);
                goto end;
            }
            ret = 1;
            goto end;
        }
        if ((*(p++) != SSL2_MT_CLIENT_CERTIFICATE) || (s->init_num < 6)) {
            ssl2_return_error(s, SSL2_PE_UNDEFINED_ERROR);
            SSLerr(SSL_F_REQUEST_CERTIFICATE, SSL_R_SHORT_READ);
            goto end;
        }
        if (s->init_num != 6) {
            SSLerr(SSL_F_REQUEST_CERTIFICATE, ERR_R_INTERNAL_ERROR);
            goto end;
        }

         
         
        ctype = *(p++);
        if (ctype != SSL2_AT_MD5_WITH_RSA_ENCRYPTION) {
            ssl2_return_error(s, SSL2_PE_UNSUPPORTED_CERTIFICATE_TYPE);
            SSLerr(SSL_F_REQUEST_CERTIFICATE, SSL_R_BAD_RESPONSE_ARGUMENT);
            goto end;
        }
        n2s(p, i);
        s->s2->tmp.clen = i;
        n2s(p, i);
        s->s2->tmp.rlen = i;
        s->state = SSL2_ST_SEND_REQUEST_CERTIFICATE_D;
    }

     
    p = (unsigned char *)s->init_buf->data;
    len = 6 + (unsigned long)s->s2->tmp.clen + (unsigned long)s->s2->tmp.rlen;
    if (len > SSL2_MAX_RECORD_LENGTH_3_BYTE_HEADER) {
        SSLerr(SSL_F_REQUEST_CERTIFICATE, SSL_R_MESSAGE_TOO_LONG);
        goto end;
    }
    j = (int)len - s->init_num;
    i = ssl2_read(s, (char *)&(p[s->init_num]), j);
    if (i < j) {
        ret = ssl2_part_read(s, SSL_F_REQUEST_CERTIFICATE, i);
        goto end;
    }
    if (s->msg_callback) {
         
        s->msg_callback(0, s->version, 0, p, len, s, s->msg_callback_arg);
    }
    p += 6;

    cp = p;
    x509 = (X509 *)d2i_X509(NULL, &cp, (long)s->s2->tmp.clen);
    if (x509 == NULL) {
        SSLerr(SSL_F_REQUEST_CERTIFICATE, ERR_R_X509_LIB);
        goto msg_end;
    }

    if (((sk = sk_X509_new_null()) == NULL) || (!sk_X509_push(sk, x509))) {
        SSLerr(SSL_F_REQUEST_CERTIFICATE, ERR_R_MALLOC_FAILURE);
        goto msg_end;
    }

    i = ssl_verify_cert_chain(s, sk);

    if (i > 0) {                 
        EVP_MD_CTX ctx;
        EVP_PKEY *pkey = NULL;

        EVP_MD_CTX_init(&ctx);
        if (!EVP_VerifyInit_ex(&ctx, s->ctx->rsa_md5, NULL)
            || !EVP_VerifyUpdate(&ctx, s->s2->key_material,
                                 s->s2->key_material_length)
            || !EVP_VerifyUpdate(&ctx, ccd, SSL2_MIN_CERT_CHALLENGE_LENGTH))
            goto msg_end;

        i = i2d_X509(s->cert->pkeys[SSL_PKEY_RSA_ENC].x509, NULL);
        buf2 = OPENSSL_malloc((unsigned int)i);
        if (buf2 == NULL) {
            SSLerr(SSL_F_REQUEST_CERTIFICATE, ERR_R_MALLOC_FAILURE);
            goto msg_end;
        }
        p2 = buf2;
        i = i2d_X509(s->cert->pkeys[SSL_PKEY_RSA_ENC].x509, &p2);
        if (!EVP_VerifyUpdate(&ctx, buf2, (unsigned int)i)) {
            OPENSSL_free(buf2);
            goto msg_end;
        }
        OPENSSL_free(buf2);

        pkey = X509_get_pubkey(x509);
        if (pkey == NULL)
            goto end;
        i = EVP_VerifyFinal(&ctx, cp, s->s2->tmp.rlen, pkey);
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_cleanup(&ctx);

        if (i > 0) {
            if (s->session->peer != NULL)
                X509_free(s->session->peer);
            s->session->peer = x509;
            CRYPTO_add(&x509->references, 1, CRYPTO_LOCK_X509);
            s->session->verify_result = s->verify_result;
            ret = 1;
            goto end;
        } else {
            SSLerr(SSL_F_REQUEST_CERTIFICATE, SSL_R_BAD_CHECKSUM);
            goto msg_end;
        }
    } else {
 msg_end:
        ssl2_return_error(s, SSL2_PE_BAD_CERTIFICATE);
    }
 end:
    sk_X509_free(sk);
    X509_free(x509);
    return (ret);
}
