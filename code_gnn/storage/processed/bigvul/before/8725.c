static int check_cert(X509_STORE_CTX *ctx)
{
    X509_CRL *crl = NULL, *dcrl = NULL;
    X509 *x;
    int ok, cnum;
    unsigned int last_reasons;
    cnum = ctx->error_depth;
    x = sk_X509_value(ctx->chain, cnum);
    ctx->current_cert = x;
    ctx->current_issuer = NULL;
    ctx->current_crl_score = 0;
    ctx->current_reasons = 0;
    if (x->ex_flags & EXFLAG_PROXY)
        return 1;
    while (ctx->current_reasons != CRLDP_ALL_REASONS) {
        last_reasons = ctx->current_reasons;
         
        if (ctx->get_crl)
            ok = ctx->get_crl(ctx, &crl, x);
        else
            ok = get_crl_delta(ctx, &crl, &dcrl, x);
         
        if (!ok) {
            ctx->error = X509_V_ERR_UNABLE_TO_GET_CRL;
            ok = ctx->verify_cb(0, ctx);
            goto err;
        }
        ctx->current_crl = crl;
        ok = ctx->check_crl(ctx, crl);
        if (!ok)
            goto err;

        if (dcrl) {
            ok = ctx->check_crl(ctx, dcrl);
            if (!ok)
                goto err;
            ok = ctx->cert_crl(ctx, dcrl, x);
            if (!ok)
                goto err;
        } else
            ok = 1;

         
        if (ok != 2) {
            ok = ctx->cert_crl(ctx, crl, x);
            if (!ok)
                goto err;
        }

        X509_CRL_free(crl);
        X509_CRL_free(dcrl);
        crl = NULL;
        dcrl = NULL;
         
        if (last_reasons == ctx->current_reasons) {
            ctx->error = X509_V_ERR_UNABLE_TO_GET_CRL;
            ok = ctx->verify_cb(0, ctx);
            goto err;
        }
    }
 err:
    X509_CRL_free(crl);
    X509_CRL_free(dcrl);

    ctx->current_crl = NULL;
    return ok;

}
