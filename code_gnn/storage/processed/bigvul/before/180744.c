 static int nft_flush_table(struct nft_ctx *ctx)
 {
 	int err;
  	struct nft_chain *chain, *nc;
  	struct nft_set *set, *ns;
  
	list_for_each_entry_safe(chain, nc, &ctx->table->chains, list) {
// 	list_for_each_entry(chain, &ctx->table->chains, list) {
  		ctx->chain = chain;
  
  		err = nft_delrule_by_chain(ctx);
  		if (err < 0)
  			goto out;
		err = nft_delchain(ctx);
		if (err < 0)
			goto out;
  	}
  
  	list_for_each_entry_safe(set, ns, &ctx->table->sets, list) {
 		if (set->flags & NFT_SET_ANONYMOUS &&
 		    !list_empty(&set->bindings))
 			continue;
 
 		err = nft_delset(ctx, set);
 		if (err < 0)
  			goto out;
  	}
  
// 	list_for_each_entry_safe(chain, nc, &ctx->table->chains, list) {
// 		ctx->chain = chain;
// 
// 		err = nft_delchain(ctx);
// 		if (err < 0)
// 			goto out;
// 	}
// 
  	err = nft_deltable(ctx);
  out:
  	return err;
 }