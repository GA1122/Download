piv_set_security_env(sc_card_t *card, const sc_security_env_t *env, int se_num)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int r = 0;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	sc_log(card->ctx,
	       "flags=%08lx op=%d alg=%d algf=%08x algr=%08x kr0=%02x, krfl=%"SC_FORMAT_LEN_SIZE_T"u",
	       env->flags, env->operation, env->algorithm, env->algorithm_flags,
	       env->algorithm_ref, env->key_ref[0], env->key_ref_len);

	priv->operation = env->operation;
	priv->algorithm = env->algorithm;

	if (env->algorithm == SC_ALGORITHM_RSA) {
		priv->alg_id = 0x06;  
	} else if (env->algorithm == SC_ALGORITHM_EC) {
		if (env->flags & SC_SEC_ENV_ALG_REF_PRESENT) {
			switch (env->algorithm_ref) {
				case 256:
					priv->alg_id = 0x11;  
					priv->key_size = 256;
					break;
				case 384:
					priv->alg_id = 0x14;
					priv->key_size = 384;
					break;
				default:
					r = SC_ERROR_NO_CARD_SUPPORT;
			}
		} else
			r = SC_ERROR_NO_CARD_SUPPORT;
	} else
		 r = SC_ERROR_NO_CARD_SUPPORT;
	priv->key_ref = env->key_ref[0];

	LOG_FUNC_RETURN(card->ctx, r);
}