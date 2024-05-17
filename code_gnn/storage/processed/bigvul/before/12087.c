int ec_precompute_mont_data(EC_GROUP *group)
{
    BN_CTX *ctx = BN_CTX_new();
    int ret = 0;

    if (group->mont_data) {
        BN_MONT_CTX_free(group->mont_data);
        group->mont_data = NULL;
    }

    if (ctx == NULL)
        goto err;

    group->mont_data = BN_MONT_CTX_new();
    if (!group->mont_data)
        goto err;

    if (!BN_MONT_CTX_set(group->mont_data, &group->order, ctx)) {
        BN_MONT_CTX_free(group->mont_data);
        group->mont_data = NULL;
        goto err;
    }

    ret = 1;

 err:

    if (ctx)
        BN_CTX_free(ctx);
    return ret;
}
