static int check_name_constraints(X509_STORE_CTX *ctx)
{
    X509 *x;
    int i, j, rv;
     
    for (i = sk_X509_num(ctx->chain) - 1; i >= 0; i--) {
        x = sk_X509_value(ctx->chain, i);
         
        if (i && (x->ex_flags & EXFLAG_SI))
            continue;
         
        for (j = sk_X509_num(ctx->chain) - 1; j > i; j--) {
            NAME_CONSTRAINTS *nc = sk_X509_value(ctx->chain, j)->nc;
            if (nc) {
                rv = NAME_CONSTRAINTS_check(x, nc);
                if (rv != X509_V_OK) {
                    ctx->error = rv;
                    ctx->error_depth = i;
                    ctx->current_cert = x;
                    if (!ctx->verify_cb(0, ctx))
                        return 0;
                }
            }
        }
    }
    return 1;
}
