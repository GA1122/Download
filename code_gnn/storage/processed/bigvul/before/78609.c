pgp_set_security_env(sc_card_t *card,
		const sc_security_env_t *env, int se_num)
{
	struct pgp_priv_data *priv = DRVDATA(card);

	LOG_FUNC_CALLED(card->ctx);

	if ((env->flags & SC_SEC_ENV_ALG_PRESENT) && (env->algorithm != SC_ALGORITHM_RSA))
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"only RSA algorithm supported");

	if (!(env->flags & SC_SEC_ENV_KEY_REF_PRESENT) || (env->key_ref_len != 1))
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"exactly one key reference required");

	if (env->flags & SC_SEC_ENV_FILE_REF_PRESENT)
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
			"passing file references not supported");

	sc_log(card->ctx, "Key ref %d", env->key_ref[0]);
	switch (env->operation) {
	case SC_SEC_OPERATION_SIGN:
		sc_log(card->ctx, "Operation: Sign.");
		if (env->key_ref[0] != 0x00 && env->key_ref[0] != 0x02) {
			LOG_TEST_RET(card->ctx, SC_ERROR_NOT_SUPPORTED,
				"Key reference not compatible with "
				"requested usage");
		}
		break;
	case SC_SEC_OPERATION_DECIPHER:
		sc_log(card->ctx, "Operation: Decipher.");
		 
		if (env->key_ref[0] != 0x01 && env->key_ref[0] != 0x02) {
			LOG_TEST_RET(card->ctx, SC_ERROR_NOT_SUPPORTED,
				"Key reference not compatible with "
				"requested usage");
		}
		break;
	default:
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid operation");
	}

	priv->sec_env = *env;

	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
}