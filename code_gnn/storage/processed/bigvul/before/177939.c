 int X509_verify_cert(X509_STORE_CTX *ctx)
 {
     X509 *x, *xtmp, *xtmp2, *chain_ss = NULL;
     int bad_chain = 0;
     X509_VERIFY_PARAM *param = ctx->param;
     int depth, i, ok = 0;
     int num, j, retry;
     int (*cb) (int xok, X509_STORE_CTX *xctx);
     STACK_OF(X509) *sktmp = NULL;
     if (ctx->cert == NULL) {
         X509err(X509_F_X509_VERIFY_CERT, X509_R_NO_CERT_SET_FOR_US_TO_VERIFY);
         return -1;
     }
 
     cb = ctx->verify_cb;
 
      
     if (ctx->chain == NULL) {
         if (((ctx->chain = sk_X509_new_null()) == NULL) ||
             (!sk_X509_push(ctx->chain, ctx->cert))) {
             X509err(X509_F_X509_VERIFY_CERT, ERR_R_MALLOC_FAILURE);
             goto end;
         }
         CRYPTO_add(&ctx->cert->references, 1, CRYPTO_LOCK_X509);
         ctx->last_untrusted = 1;
     }
 
      
     if (ctx->untrusted != NULL
         && (sktmp = sk_X509_dup(ctx->untrusted)) == NULL) {
         X509err(X509_F_X509_VERIFY_CERT, ERR_R_MALLOC_FAILURE);
         goto end;
     }
 
     num = sk_X509_num(ctx->chain);
     x = sk_X509_value(ctx->chain, num - 1);
     depth = param->depth;
 
     for (;;) {
          
         if (depth < num)
             break;               
 
          
         if (ctx->check_issued(ctx, x, x))
             break;
 
          
         if (ctx->untrusted != NULL) {
             xtmp = find_issuer(ctx, sktmp, x);
             if (xtmp != NULL) {
                 if (!sk_X509_push(ctx->chain, xtmp)) {
                     X509err(X509_F_X509_VERIFY_CERT, ERR_R_MALLOC_FAILURE);
                     goto end;
                 }
                 CRYPTO_add(&xtmp->references, 1, CRYPTO_LOCK_X509);
                 (void)sk_X509_delete_ptr(sktmp, xtmp);
                 ctx->last_untrusted++;
                 x = xtmp;
                 num++;
                  
                 continue;
             }
         }
         break;
     }
 
      
     j = num;
      
 
     do {
          
         i = sk_X509_num(ctx->chain);
         x = sk_X509_value(ctx->chain, i - 1);
         if (ctx->check_issued(ctx, x, x)) {
              
             if (sk_X509_num(ctx->chain) == 1) {
                  
                 ok = ctx->get_issuer(&xtmp, ctx, x);
                 if ((ok <= 0) || X509_cmp(x, xtmp)) {
                     ctx->error = X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT;
                     ctx->current_cert = x;
                     ctx->error_depth = i - 1;
                     if (ok == 1)
                         X509_free(xtmp);
                     bad_chain = 1;
                     ok = cb(0, ctx);
                     if (!ok)
                         goto end;
                 } else {
                      
                     X509_free(x);
                     x = xtmp;
                     (void)sk_X509_set(ctx->chain, i - 1, x);
                     ctx->last_untrusted = 0;
                 }
             } else {
                  
                 chain_ss = sk_X509_pop(ctx->chain);
                 ctx->last_untrusted--;
                 num--;
                 j--;
                 x = sk_X509_value(ctx->chain, num - 1);
             }
         }
          
         for (;;) {
              
             if (depth < num)
                 break;
              
             if (ctx->check_issued(ctx, x, x))
                 break;
             ok = ctx->get_issuer(&xtmp, ctx, x);
             if (ok < 0)
                 return ok;
             if (ok == 0)
                 break;
             x = xtmp;
             if (!sk_X509_push(ctx->chain, x)) {
                 X509_free(xtmp);
                 X509err(X509_F_X509_VERIFY_CERT, ERR_R_MALLOC_FAILURE);
                 return 0;
             }
             num++;
         }
 
          
         retry = 0;
         if (j == ctx->last_untrusted &&
             !(ctx->param->flags & X509_V_FLAG_NO_ALT_CHAINS)) {
             while (j-- > 1) {
                 xtmp2 = sk_X509_value(ctx->chain, j - 1);
                 ok = ctx->get_issuer(&xtmp, ctx, xtmp2);
                 if (ok < 0)
                     goto end;
                  
                 if (ok > 0) {
                      
                     X509_free(xtmp);
 
                      
                     while (num > j) {
                          xtmp = sk_X509_pop(ctx->chain);
                          X509_free(xtmp);
                          num--;
                        ctx->last_untrusted--;
                      }
//                     ctx->last_untrusted = sk_X509_num(ctx->chain);
                      retry = 1;
                      break;
                  }
             }
         }
     } while (retry);
 
      
     if (!ctx->check_issued(ctx, x, x)) {
         if ((chain_ss == NULL) || !ctx->check_issued(ctx, x, chain_ss)) {
             if (ctx->last_untrusted >= num)
                 ctx->error = X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY;
             else
                 ctx->error = X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT;
             ctx->current_cert = x;
         } else {
 
             sk_X509_push(ctx->chain, chain_ss);
             num++;
             ctx->last_untrusted = num;
             ctx->current_cert = chain_ss;
             ctx->error = X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN;
             chain_ss = NULL;
         }
 
         ctx->error_depth = num - 1;
         bad_chain = 1;
         ok = cb(0, ctx);
         if (!ok)
             goto end;
     }
 
      
     ok = check_chain_extensions(ctx);
 
     if (!ok)
         goto end;
 
      
 
     ok = check_name_constraints(ctx);
 
     if (!ok)
         goto end;
 
      
 
     if (param->trust > 0)
         ok = check_trust(ctx);
 
     if (!ok)
         goto end;
 
      
     X509_get_pubkey_parameters(NULL, ctx->chain);
 
      
 
     ok = ctx->check_revocation(ctx);
     if (!ok)
         goto end;
 
      
     if (ctx->verify != NULL)
         ok = ctx->verify(ctx);
     else
         ok = internal_verify(ctx);
     if (!ok)
         goto end;
 
 #ifndef OPENSSL_NO_RFC3779
      
     ok = v3_asid_validate_path(ctx);
     if (!ok)
         goto end;
     ok = v3_addr_validate_path(ctx);
     if (!ok)
         goto end;
 #endif
 
      
     if (!bad_chain && (ctx->param->flags & X509_V_FLAG_POLICY_CHECK))
         ok = ctx->check_policy(ctx);
     if (!ok)
         goto end;
     if (0) {
  end:
         X509_get_pubkey_parameters(NULL, ctx->chain);
     }
     if (sktmp != NULL)
         sk_X509_free(sktmp);
     if (chain_ss != NULL)
         X509_free(chain_ss);
     return ok;
 }