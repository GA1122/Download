static int check_chain_extensions(X509_STORE_CTX *ctx)
{
#ifdef OPENSSL_NO_CHAIN_VERIFY
    return 1;
#else
    int i, ok = 0, must_be_ca, plen = 0;
    X509 *x;
    int (*cb) (int xok, X509_STORE_CTX *xctx);
    int proxy_path_length = 0;
    int purpose;
    int allow_proxy_certs;
    cb = ctx->verify_cb;

     
    must_be_ca = -1;

     
    if (ctx->parent) {
        allow_proxy_certs = 0;
        purpose = X509_PURPOSE_CRL_SIGN;
    } else {
        allow_proxy_certs =
            ! !(ctx->param->flags & X509_V_FLAG_ALLOW_PROXY_CERTS);
         
        if (getenv("OPENSSL_ALLOW_PROXY_CERTS"))
            allow_proxy_certs = 1;
        purpose = ctx->param->purpose;
    }

     
    for (i = 0; i < ctx->last_untrusted; i++) {
        int ret;
        x = sk_X509_value(ctx->chain, i);
        if (!(ctx->param->flags & X509_V_FLAG_IGNORE_CRITICAL)
            && (x->ex_flags & EXFLAG_CRITICAL)) {
            ctx->error = X509_V_ERR_UNHANDLED_CRITICAL_EXTENSION;
            ctx->error_depth = i;
            ctx->current_cert = x;
            ok = cb(0, ctx);
            if (!ok)
                goto end;
        }
        if (!allow_proxy_certs && (x->ex_flags & EXFLAG_PROXY)) {
            ctx->error = X509_V_ERR_PROXY_CERTIFICATES_NOT_ALLOWED;
            ctx->error_depth = i;
            ctx->current_cert = x;
            ok = cb(0, ctx);
            if (!ok)
                goto end;
        }
        ret = X509_check_ca(x);
        switch (must_be_ca) {
        case -1:
            if ((ctx->param->flags & X509_V_FLAG_X509_STRICT)
                && (ret != 1) && (ret != 0)) {
                ret = 0;
                ctx->error = X509_V_ERR_INVALID_CA;
            } else
                ret = 1;
            break;
        case 0:
            if (ret != 0) {
                ret = 0;
                ctx->error = X509_V_ERR_INVALID_NON_CA;
            } else
                ret = 1;
            break;
        default:
            if ((ret == 0)
                || ((ctx->param->flags & X509_V_FLAG_X509_STRICT)
                    && (ret != 1))) {
                ret = 0;
                ctx->error = X509_V_ERR_INVALID_CA;
            } else
                ret = 1;
            break;
        }
        if (ret == 0) {
            ctx->error_depth = i;
            ctx->current_cert = x;
            ok = cb(0, ctx);
            if (!ok)
                goto end;
        }
        if (ctx->param->purpose > 0) {
            ret = X509_check_purpose(x, purpose, must_be_ca > 0);
            if ((ret == 0)
                || ((ctx->param->flags & X509_V_FLAG_X509_STRICT)
                    && (ret != 1))) {
                ctx->error = X509_V_ERR_INVALID_PURPOSE;
                ctx->error_depth = i;
                ctx->current_cert = x;
                ok = cb(0, ctx);
                if (!ok)
                    goto end;
            }
        }
         
        if ((i > 1) && !(x->ex_flags & EXFLAG_SI)
            && (x->ex_pathlen != -1)
            && (plen > (x->ex_pathlen + proxy_path_length + 1))) {
            ctx->error = X509_V_ERR_PATH_LENGTH_EXCEEDED;
            ctx->error_depth = i;
            ctx->current_cert = x;
            ok = cb(0, ctx);
            if (!ok)
                goto end;
        }
         
        if (!(x->ex_flags & EXFLAG_SI))
            plen++;
         
        if (x->ex_flags & EXFLAG_PROXY) {
            if (x->ex_pcpathlen != -1 && i > x->ex_pcpathlen) {
                ctx->error = X509_V_ERR_PROXY_PATH_LENGTH_EXCEEDED;
                ctx->error_depth = i;
                ctx->current_cert = x;
                ok = cb(0, ctx);
                if (!ok)
                    goto end;
            }
            proxy_path_length++;
            must_be_ca = 0;
        } else
            must_be_ca = 1;
    }
    ok = 1;
 end:
    return ok;
#endif
}
