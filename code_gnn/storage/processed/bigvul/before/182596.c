 static int pcrypt_create_aead(struct crypto_template *tmpl, struct rtattr **tb,
 			      u32 type, u32 mask)
 {
 	struct pcrypt_instance_ctx *ctx;
 	struct crypto_attr_type *algt;
 	struct aead_instance *inst;
 	struct aead_alg *alg;
 	const char *name;
 	int err;
 
 	algt = crypto_get_attr_type(tb);
 	if (IS_ERR(algt))
 		return PTR_ERR(algt);
 
 	name = crypto_attr_alg_name(tb[1]);
 	if (IS_ERR(name))
 		return PTR_ERR(name);
 
 	inst = kzalloc(sizeof(*inst) + sizeof(*ctx), GFP_KERNEL);
 	if (!inst)
 		return -ENOMEM;
 
 	ctx = aead_instance_ctx(inst);
 	crypto_set_aead_spawn(&ctx->spawn, aead_crypto_instance(inst));
 
 	err = crypto_grab_aead(&ctx->spawn, name, 0, 0);
 	if (err)
 		goto out_free_inst;
 
 	alg = crypto_spawn_aead_alg(&ctx->spawn);
 	err = pcrypt_init_instance(aead_crypto_instance(inst), &alg->base);
 	if (err)
 		goto out_drop_aead;
 
 	inst->alg.base.cra_flags = CRYPTO_ALG_ASYNC;
 
 	inst->alg.ivsize = crypto_aead_alg_ivsize(alg);
 	inst->alg.maxauthsize = crypto_aead_alg_maxauthsize(alg);
 
 	inst->alg.base.cra_ctxsize = sizeof(struct pcrypt_aead_ctx);
 
 	inst->alg.init = pcrypt_aead_init_tfm;
 	inst->alg.exit = pcrypt_aead_exit_tfm;
 
 	inst->alg.setkey = pcrypt_aead_setkey;
 	inst->alg.setauthsize = pcrypt_aead_setauthsize;
  	inst->alg.encrypt = pcrypt_aead_encrypt;
  	inst->alg.decrypt = pcrypt_aead_decrypt;
  
// 	inst->free = pcrypt_free;
// 
  	err = aead_register_instance(tmpl, inst);
  	if (err)
  		goto out_drop_aead;
 
 out:
 	return err;
 
 out_drop_aead:
 	crypto_drop_aead(&ctx->spawn);
 out_free_inst:
 	kfree(inst);
 	goto out;
 }