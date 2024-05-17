int BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx)
{
    int i, j, bits, ret = 0, wstart, wend, window, wvalue;
    int start = 1;
    BIGNUM *aa;
     
    BIGNUM *val[TABLE_SIZE];
    BN_RECP_CTX recp;

    if (BN_get_flags(p, BN_FLG_CONSTTIME) != 0) {
         
        BNerr(BN_F_BN_MOD_EXP_RECP, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return -1;
    }

    bits = BN_num_bits(p);
    if (bits == 0) {
         
        if (BN_is_one(m)) {
            ret = 1;
            BN_zero(r);
        } else {
            ret = BN_one(r);
        }
        return ret;
    }

    BN_CTX_start(ctx);
    aa = BN_CTX_get(ctx);
    val[0] = BN_CTX_get(ctx);
    if (!aa || !val[0])
        goto err;

    BN_RECP_CTX_init(&recp);
    if (m->neg) {
         
        if (!BN_copy(aa, m))
            goto err;
        aa->neg = 0;
        if (BN_RECP_CTX_set(&recp, aa, ctx) <= 0)
            goto err;
    } else {
        if (BN_RECP_CTX_set(&recp, m, ctx) <= 0)
            goto err;
    }

    if (!BN_nnmod(val[0], a, m, ctx))
        goto err;                
    if (BN_is_zero(val[0])) {
        BN_zero(r);
        ret = 1;
        goto err;
    }

    window = BN_window_bits_for_exponent_size(bits);
    if (window > 1) {
        if (!BN_mod_mul_reciprocal(aa, val[0], val[0], &recp, ctx))
            goto err;            
        j = 1 << (window - 1);
        for (i = 1; i < j; i++) {
            if (((val[i] = BN_CTX_get(ctx)) == NULL) ||
                !BN_mod_mul_reciprocal(val[i], val[i - 1], aa, &recp, ctx))
                goto err;
        }
    }

    start = 1;                   
    wvalue = 0;                  
    wstart = bits - 1;           
    wend = 0;                    

    if (!BN_one(r))
        goto err;

    for (;;) {
        if (BN_is_bit_set(p, wstart) == 0) {
            if (!start)
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx))
                    goto err;
            if (wstart == 0)
                break;
            wstart--;
            continue;
        }
         
        j = wstart;
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0)
                break;
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

         
        j = wend + 1;
         
        if (!start)
            for (i = 0; i < j; i++) {
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx))
                    goto err;
            }

         
        if (!BN_mod_mul_reciprocal(r, r, val[wvalue >> 1], &recp, ctx))
            goto err;

         
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0)
            break;
    }
    ret = 1;
 err:
    BN_CTX_end(ctx);
    BN_RECP_CTX_free(&recp);
    bn_check_top(r);
    return (ret);
}