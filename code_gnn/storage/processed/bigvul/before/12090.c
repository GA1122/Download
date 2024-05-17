int EC_GROUP_copy(EC_GROUP *dest, const EC_GROUP *src)
{
    if (dest->meth->group_copy == 0) {
        ECerr(EC_F_EC_GROUP_COPY, ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }
    if (dest->meth != src->meth) {
        ECerr(EC_F_EC_GROUP_COPY, EC_R_INCOMPATIBLE_OBJECTS);
        return 0;
    }
    if (dest == src)
        return 1;

    dest->curve_name = src->curve_name;

     
    dest->pre_comp_type = src->pre_comp_type;
    switch (src->pre_comp_type) {
    default:
        dest->pre_comp.ec = NULL;
        break;
#ifdef ECP_NISTZ256_REFERENCE_IMPLEMENTATION
    case PCT_nistz256:
        dest->pre_comp.nistz256 = EC_nistz256_pre_comp_dup(src->pre_comp.nistz256);
        break;
#endif
#ifndef OPENSSL_NO_EC_NISTP_64_GCC_128
    case PCT_nistp224:
        dest->pre_comp.nistp224 = EC_nistp224_pre_comp_dup(src->pre_comp.nistp224);
        break;
    case PCT_nistp256:
        dest->pre_comp.nistp256 = EC_nistp256_pre_comp_dup(src->pre_comp.nistp256);
        break;
    case PCT_nistp521:
        dest->pre_comp.nistp521 = EC_nistp521_pre_comp_dup(src->pre_comp.nistp521);
        break;
#endif
    case PCT_ec:
        dest->pre_comp.ec = EC_ec_pre_comp_dup(src->pre_comp.ec);
        break;
    }

    if (src->mont_data != NULL) {
        if (dest->mont_data == NULL) {
            dest->mont_data = BN_MONT_CTX_new();
            if (dest->mont_data == NULL)
                return 0;
        }
        if (!BN_MONT_CTX_copy(dest->mont_data, src->mont_data))
            return 0;
    } else {
         
        BN_MONT_CTX_free(dest->mont_data);
        dest->mont_data = NULL;
    }

    if (src->generator != NULL) {
        if (dest->generator == NULL) {
            dest->generator = EC_POINT_new(dest);
            if (dest->generator == NULL)
                return 0;
        }
        if (!EC_POINT_copy(dest->generator, src->generator))
            return 0;
    } else {
         
        EC_POINT_clear_free(dest->generator);
        dest->generator = NULL;
    }

    if ((src->meth->flags & EC_FLAGS_CUSTOM_CURVE) == 0) {
        if (!BN_copy(dest->order, src->order))
            return 0;
        if (!BN_copy(dest->cofactor, src->cofactor))
            return 0;
    }

    dest->asn1_flag = src->asn1_flag;
    dest->asn1_form = src->asn1_form;

    if (src->seed) {
        OPENSSL_free(dest->seed);
        dest->seed = OPENSSL_malloc(src->seed_len);
        if (dest->seed == NULL)
            return 0;
        if (!memcpy(dest->seed, src->seed, src->seed_len))
            return 0;
        dest->seed_len = src->seed_len;
    } else {
        OPENSSL_free(dest->seed);
        dest->seed = NULL;
        dest->seed_len = 0;
    }

    return dest->meth->group_copy(dest, src);
}
