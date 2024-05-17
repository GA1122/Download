 int ec_scalar_mul_ladder(const EC_GROUP *group, EC_POINT *r,
                          const BIGNUM *scalar, const EC_POINT *point,
                          BN_CTX *ctx)
 {
     int i, cardinality_bits, group_top, kbit, pbit, Z_is_one;
     EC_POINT *p = NULL;
     EC_POINT *s = NULL;
     BIGNUM *k = NULL;
     BIGNUM *lambda = NULL;
     BIGNUM *cardinality = NULL;
     int ret = 0;
 
      
     if (point != NULL && EC_POINT_is_at_infinity(group, point))
         return EC_POINT_set_to_infinity(group, r);
 
     if (BN_is_zero(group->order)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, EC_R_UNKNOWN_ORDER);
         return 0;
     }
     if (BN_is_zero(group->cofactor)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, EC_R_UNKNOWN_COFACTOR);
         return 0;
     }
 
     BN_CTX_start(ctx);
 
     if (((p = EC_POINT_new(group)) == NULL)
         || ((s = EC_POINT_new(group)) == NULL)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_MALLOC_FAILURE);
         goto err;
     }
 
     if (point == NULL) {
         if (!EC_POINT_copy(p, group->generator)) {
             ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_EC_LIB);
             goto err;
         }
     } else {
         if (!EC_POINT_copy(p, point)) {
             ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_EC_LIB);
             goto err;
         }
     }
 
     EC_POINT_BN_set_flags(p, BN_FLG_CONSTTIME);
     EC_POINT_BN_set_flags(r, BN_FLG_CONSTTIME);
     EC_POINT_BN_set_flags(s, BN_FLG_CONSTTIME);
 
     cardinality = BN_CTX_get(ctx);
     lambda = BN_CTX_get(ctx);
     k = BN_CTX_get(ctx);
     if (k == NULL) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_MALLOC_FAILURE);
         goto err;
     }
 
     if (!BN_mul(cardinality, group->order, group->cofactor, ctx)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
         goto err;
     }
 
      
      cardinality_bits = BN_num_bits(cardinality);
      group_top = bn_get_top(cardinality);
    if ((bn_wexpand(k, group_top + 1) == NULL)
        || (bn_wexpand(lambda, group_top + 1) == NULL)) {
//     if ((bn_wexpand(k, group_top + 2) == NULL)
//         || (bn_wexpand(lambda, group_top + 2) == NULL)) {
          ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
          goto err;
      }
 
     if (!BN_copy(k, scalar)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
         goto err;
     }
 
     BN_set_flags(k, BN_FLG_CONSTTIME);
 
     if ((BN_num_bits(k) > cardinality_bits) || (BN_is_negative(k))) {
          
         if (!BN_nnmod(k, k, cardinality, ctx)) {
             ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
             goto err;
         }
     }
 
     if (!BN_add(lambda, k, cardinality)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
         goto err;
     }
     BN_set_flags(lambda, BN_FLG_CONSTTIME);
     if (!BN_add(k, lambda, cardinality)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
         goto err;
     }
      
      kbit = BN_is_bit_set(lambda, cardinality_bits);
    BN_consttime_swap(kbit, k, lambda, group_top + 1);
//     BN_consttime_swap(kbit, k, lambda, group_top + 2);
  
      group_top = bn_get_top(group->field);
      if ((bn_wexpand(s->X, group_top) == NULL)
         || (bn_wexpand(s->Y, group_top) == NULL)
         || (bn_wexpand(s->Z, group_top) == NULL)
         || (bn_wexpand(r->X, group_top) == NULL)
         || (bn_wexpand(r->Y, group_top) == NULL)
         || (bn_wexpand(r->Z, group_top) == NULL)
         || (bn_wexpand(p->X, group_top) == NULL)
         || (bn_wexpand(p->Y, group_top) == NULL)
         || (bn_wexpand(p->Z, group_top) == NULL)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, ERR_R_BN_LIB);
         goto err;
     }
 
      
     if (!ec_point_blind_coordinates(group, p, ctx)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, EC_R_POINT_COORDINATES_BLIND_FAILURE);
         goto err;
     }
 
      
     if (!ec_point_ladder_pre(group, r, s, p, ctx)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, EC_R_LADDER_PRE_FAILURE);
         goto err;
     }
 
      
     pbit = 1;
 
 #define EC_POINT_CSWAP(c, a, b, w, t) do {         \
         BN_consttime_swap(c, (a)->X, (b)->X, w);   \
         BN_consttime_swap(c, (a)->Y, (b)->Y, w);   \
         BN_consttime_swap(c, (a)->Z, (b)->Z, w);   \
         t = ((a)->Z_is_one ^ (b)->Z_is_one) & (c); \
         (a)->Z_is_one ^= (t);                      \
         (b)->Z_is_one ^= (t);                      \
 } while(0)
 
      
 
     for (i = cardinality_bits - 1; i >= 0; i--) {
         kbit = BN_is_bit_set(k, i) ^ pbit;
         EC_POINT_CSWAP(kbit, r, s, group_top, Z_is_one);
 
          
         if (!ec_point_ladder_step(group, r, s, p, ctx)) {
             ECerr(EC_F_EC_SCALAR_MUL_LADDER, EC_R_LADDER_STEP_FAILURE);
             goto err;
         }
          
         pbit ^= kbit;
     }
      
     EC_POINT_CSWAP(pbit, r, s, group_top, Z_is_one);
 #undef EC_POINT_CSWAP
 
      
     if (!ec_point_ladder_post(group, r, s, p, ctx)) {
         ECerr(EC_F_EC_SCALAR_MUL_LADDER, EC_R_LADDER_POST_FAILURE);
         goto err;
     }
 
     ret = 1;
 
  err:
     EC_POINT_free(p);
     EC_POINT_free(s);
     BN_CTX_end(ctx);
 
     return ret;
 }