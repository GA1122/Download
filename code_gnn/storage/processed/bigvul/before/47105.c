static int _get_more_prng_bytes(struct prng_context *ctx, int cont_test)
{
	int i;
	unsigned char tmp[DEFAULT_BLK_SZ];
	unsigned char *output = NULL;


	dbgprint(KERN_CRIT "Calling _get_more_prng_bytes for context %p\n",
		ctx);

	hexdump("Input DT: ", ctx->DT, DEFAULT_BLK_SZ);
	hexdump("Input I: ", ctx->I, DEFAULT_BLK_SZ);
	hexdump("Input V: ", ctx->V, DEFAULT_BLK_SZ);

	 
	for (i = 0; i < 3; i++) {

		switch (i) {
		case 0:
			 
			memcpy(tmp, ctx->DT, DEFAULT_BLK_SZ);
			output = ctx->I;
			hexdump("tmp stage 0: ", tmp, DEFAULT_BLK_SZ);
			break;
		case 1:

			 
			xor_vectors(ctx->I, ctx->V, tmp, DEFAULT_BLK_SZ);
			hexdump("tmp stage 1: ", tmp, DEFAULT_BLK_SZ);
			output = ctx->rand_data;
			break;
		case 2:
			 
			if (!memcmp(ctx->rand_data, ctx->last_rand_data,
					DEFAULT_BLK_SZ)) {
				if (cont_test) {
					panic("cprng %p Failed repetition check!\n",
						ctx);
				}

				printk(KERN_ERR
					"ctx %p Failed repetition check!\n",
					ctx);

				ctx->flags |= PRNG_NEED_RESET;
				return -EINVAL;
			}
			memcpy(ctx->last_rand_data, ctx->rand_data,
				DEFAULT_BLK_SZ);

			 
			xor_vectors(ctx->rand_data, ctx->I, tmp,
				DEFAULT_BLK_SZ);
			output = ctx->V;
			hexdump("tmp stage 2: ", tmp, DEFAULT_BLK_SZ);
			break;
		}


		 
		crypto_cipher_encrypt_one(ctx->tfm, output, tmp);

	}

	 
	for (i = DEFAULT_BLK_SZ - 1; i >= 0; i--) {
		ctx->DT[i] += 1;
		if (ctx->DT[i] != 0)
			break;
	}

	dbgprint("Returning new block for context %p\n", ctx);
	ctx->rand_data_valid = 0;

	hexdump("Output DT: ", ctx->DT, DEFAULT_BLK_SZ);
	hexdump("Output I: ", ctx->I, DEFAULT_BLK_SZ);
	hexdump("Output V: ", ctx->V, DEFAULT_BLK_SZ);
	hexdump("New Random Data: ", ctx->rand_data, DEFAULT_BLK_SZ);

	return 0;
}
