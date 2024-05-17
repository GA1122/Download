static DSA_SIG *dsa_do_sign(const unsigned char *dgst, int dlen, DSA *dsa)
{
    BIGNUM *kinv = NULL, *r = NULL, *s = NULL;
    BIGNUM *m, *blind, *blindm, *tmp;
    BN_CTX *ctx = NULL;
    int reason = ERR_R_BN_LIB;
    DSA_SIG *ret = NULL;
    int noredo = 0;

    if (dsa->p == NULL || dsa->q == NULL || dsa->g == NULL) {
        reason = DSA_R_MISSING_PARAMETERS;
        goto err;
    }

    s = BN_new();
    if (s == NULL)
        goto err;
    ctx = BN_CTX_new();
    if (ctx == NULL)
        goto err;
    m = BN_CTX_get(ctx);
    blind = BN_CTX_get(ctx);
    blindm = BN_CTX_get(ctx);
    tmp = BN_CTX_get(ctx);
    if (tmp == NULL)
        goto err;

 redo:
    if ((dsa->kinv == NULL) || (dsa->r == NULL)) {
        if (!DSA_sign_setup(dsa, ctx, &kinv, &r))
            goto err;
    } else {
        kinv = dsa->kinv;
        dsa->kinv = NULL;
        r = dsa->r;
        dsa->r = NULL;
        noredo = 1;
    }

    if (dlen > BN_num_bytes(dsa->q))
         
        dlen = BN_num_bytes(dsa->q);
    if (BN_bin2bn(dgst, dlen, m) == NULL)
        goto err;

     

     
    do {
        if (!BN_rand(blind, BN_num_bits(dsa->q) - 1, -1, 0))
            goto err;
    } while (BN_is_zero(blind));
    BN_set_flags(blind, BN_FLG_CONSTTIME);
    BN_set_flags(blindm, BN_FLG_CONSTTIME);
    BN_set_flags(tmp, BN_FLG_CONSTTIME);

     
    if (!BN_mod_mul(tmp, blind, dsa->priv_key, dsa->q, ctx))
        goto err;
    if (!BN_mod_mul(tmp, tmp, r, dsa->q, ctx))
        goto err;

     
    if (!BN_mod_mul(blindm, blind, m, dsa->q, ctx))
        goto err;

     
    if (!BN_mod_add_quick(s, tmp, blindm, dsa->q))
        goto err;

     
    if (!BN_mod_mul(s, s, kinv, dsa->q, ctx))
        goto err;

     
    if (BN_mod_inverse(blind, blind, dsa->q, ctx) == NULL)
        goto err;
    if (!BN_mod_mul(s, s, blind, dsa->q, ctx))
        goto err;

     
    if (BN_is_zero(r) || BN_is_zero(s)) {
        if (noredo) {
            reason = DSA_R_NEED_NEW_SETUP_VALUES;
            goto err;
        }
        goto redo;
    }
    ret = DSA_SIG_new();
    if (ret == NULL)
        goto err;
    ret->r = r;
    ret->s = s;

 err:
    if (ret == NULL) {
        DSAerr(DSA_F_DSA_DO_SIGN, reason);
        BN_free(r);
        BN_free(s);
    }
    BN_CTX_free(ctx);
    BN_clear_free(kinv);
    return ret;
}
