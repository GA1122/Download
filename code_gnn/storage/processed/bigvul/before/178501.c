 static int ec_mul_consttime(const EC_GROUP *group, EC_POINT *r,
                             const BIGNUM *scalar, const EC_POINT *point,
                             BN_CTX *ctx)
 {
     int i, cardinality_bits, group_top, kbit, pbit, Z_is_one;
     EC_POINT *s = NULL;
     BIGNUM *k = NULL;
     BIGNUM *lambda = NULL;
     BIGNUM *cardinality = NULL;
     BN_CTX *new_ctx = NULL;
     int ret = 0;
 
     if (ctx == NULL && (ctx = new_ctx = BN_CTX_secure_new()) == NULL)
         return 0;
 
     BN_CTX_start(ctx);
 
     s = EC_POINT_new(group);
     if (s == NULL)
         goto err;
 
     if (point == NULL) {
         if (!EC_POINT_copy(s, group->generator))
             goto err;
     } else {
         if (!EC_POINT_copy(s, point))
             goto err;
     }
 
     EC_POINT_BN_set_flags(s, BN_FLG_CONSTTIME);
 
     cardinality = BN_CTX_get(ctx);
     lambda = BN_CTX_get(ctx);
     k = BN_CTX_get(ctx);
     if (k == NULL || !BN_mul(cardinality, group->order, group->cofactor, ctx))
         goto err;
 
      
      cardinality_bits = BN_num_bits(cardinality);
      group_top = bn_get_top(cardinality);
    if ((bn_wexpand(k, group_top + 1) == NULL)
        || (bn_wexpand(lambda, group_top + 1) == NULL))
//     if ((bn_wexpand(k, group_top + 2) == NULL)
//         || (bn_wexpand(lambda, group_top + 2) == NULL)) {
          goto err;
  
      if (!BN_copy(k, scalar))
         goto err;
 
     BN_set_flags(k, BN_FLG_CONSTTIME);
 
     if ((BN_num_bits(k) > cardinality_bits) || (BN_is_negative(k))) {
          
         if (!BN_nnmod(k, k, cardinality, ctx))
             goto err;
     }
 
     if (!BN_add(lambda, k, cardinality))
         goto err;
     BN_set_flags(lambda, BN_FLG_CONSTTIME);
     if (!BN_add(k, lambda, cardinality))
         goto err;
      
      kbit = BN_is_bit_set(lambda, cardinality_bits);
    BN_consttime_swap(kbit, k, lambda, group_top + 1);
//     BN_consttime_swap(kbit, k, lambda, group_top + 2);
  
      group_top = bn_get_top(group->field);
      if ((bn_wexpand(s->X, group_top) == NULL)
         || (bn_wexpand(s->Y, group_top) == NULL)
         || (bn_wexpand(s->Z, group_top) == NULL)
         || (bn_wexpand(r->X, group_top) == NULL)
         || (bn_wexpand(r->Y, group_top) == NULL)
         || (bn_wexpand(r->Z, group_top) == NULL))
         goto err;
 
      
     if (!ec_point_blind_coordinates(group, s, ctx))
         goto err;
 
      
     if (!EC_POINT_copy(r, s))
         goto err;
 
     EC_POINT_BN_set_flags(r, BN_FLG_CONSTTIME);
 
     if (!EC_POINT_dbl(group, s, s, ctx))
         goto err;
 
     pbit = 0;
 
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
         if (!EC_POINT_add(group, s, r, s, ctx))
             goto err;
         if (!EC_POINT_dbl(group, r, r, ctx))
             goto err;
          
         pbit ^= kbit;
     }
      
     EC_POINT_CSWAP(pbit, r, s, group_top, Z_is_one);
 #undef EC_POINT_CSWAP
 
     ret = 1;
 
  err:
     EC_POINT_free(s);
     BN_CTX_end(ctx);
     BN_CTX_free(new_ctx);
 
     return ret;
 }