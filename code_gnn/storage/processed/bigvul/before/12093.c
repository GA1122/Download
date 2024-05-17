EC_GROUP *EC_GROUP_new(const EC_METHOD *meth)
{
    EC_GROUP *ret;

    if (meth == NULL) {
        ECerr(EC_F_EC_GROUP_NEW, EC_R_SLOT_FULL);
        return NULL;
    }
    if (meth->group_init == 0) {
        ECerr(EC_F_EC_GROUP_NEW, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return NULL;
    }

    ret = OPENSSL_zalloc(sizeof(*ret));
    if (ret == NULL) {
        ECerr(EC_F_EC_GROUP_NEW, ERR_R_MALLOC_FAILURE);
        return NULL;
    }

    ret->meth = meth;
    if ((ret->meth->flags & EC_FLAGS_CUSTOM_CURVE) == 0) {
        ret->order = BN_new();
        if (ret->order == NULL)
            goto err;
        ret->cofactor = BN_new();
        if (ret->cofactor == NULL)
            goto err;
    }
    ret->asn1_flag = OPENSSL_EC_NAMED_CURVE;
    ret->asn1_form = POINT_CONVERSION_UNCOMPRESSED;
    if (!meth->group_init(ret))
        goto err;
    return ret;

 err:
    BN_free(ret->order);
    BN_free(ret->cofactor);
    OPENSSL_free(ret);
    return NULL;
}
