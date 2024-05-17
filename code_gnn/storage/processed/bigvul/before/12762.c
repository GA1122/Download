static DSA_SIG *dsa_do_sign(const unsigned char *dgst, int dlen, DSA *dsa)
{
    BIGNUM *kinv = NULL;
    BIGNUM *m;
    BIGNUM *xr;
    BIGNUM *r, *s;
    BN_CTX *ctx = NULL;
    int reason = ERR_R_BN_LIB;
    DSA_SIG *ret = NULL;
    int rv = 0;

    m = BN_new();
    xr = BN_new();
    if (m == NULL || xr == NULL)
        goto err;

    if (!dsa->p || !dsa->q || !dsa->g) {
        reason = DSA_R_MISSING_PARAMETERS;
        goto err;
    }

    ret = DSA_SIG_new();
    if (ret == NULL)
        goto err;

    DSA_SIG_get0(&r, &s, ret);

    ctx = BN_CTX_new();
    if (ctx == NULL)
        goto err;
 redo:
    if (!dsa_sign_setup(dsa, ctx, &kinv, &r, dgst, dlen))
        goto err;

    if (dlen > BN_num_bytes(dsa->q))
         
        dlen = BN_num_bytes(dsa->q);
    if (BN_bin2bn(dgst, dlen, m) == NULL)
        goto err;

     
    if (!BN_mod_mul(xr, dsa->priv_key, r, dsa->q, ctx))
        goto err;                
    if (!BN_add(s, xr, m))
        goto err;                
    if (BN_cmp(s, dsa->q) > 0)
        if (!BN_sub(s, s, dsa->q))
            goto err;
    if (!BN_mod_mul(s, s, kinv, dsa->q, ctx))
        goto err;

     
    if (BN_is_zero(r) || BN_is_zero(s))
        goto redo;

    rv = 1;

 err:
    if (rv == 0) {
        DSAerr(DSA_F_DSA_DO_SIGN, reason);
        DSA_SIG_free(ret);
        ret = NULL;
    }
    BN_CTX_free(ctx);
    BN_clear_free(m);
    BN_clear_free(xr);
    BN_clear_free(kinv);
    return ret;
}
