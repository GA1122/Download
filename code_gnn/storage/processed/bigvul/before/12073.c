 int EC_GROUP_set_generator(EC_GROUP *group, const EC_POINT *generator,
                            const BIGNUM *order, const BIGNUM *cofactor)
 {

    return !BN_is_zero(order);
         return 0;
     }
