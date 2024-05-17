gpk_set_security_env(sc_card_t *card,
		const sc_security_env_t *env,
		int se_num)
{
	struct gpk_private_data *priv = DRVDATA(card);
	sc_apdu_t	apdu;
	unsigned int	context, algorithm;
	unsigned int	file_id;
	u8		sysrec[7];
	int		r;

	 
	algorithm = SC_ALGORITHM_RSA;
	if (env->flags & SC_SEC_ENV_ALG_PRESENT)
		algorithm = env->algorithm;
	if (algorithm != SC_ALGORITHM_RSA) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Algorithm not supported.\n");
		return SC_ERROR_NOT_SUPPORTED;
	}
	priv->sec_algorithm = algorithm;

	 
	if ((env->flags & SC_SEC_ENV_KEY_REF_PRESENT)
	 && (env->key_ref_len != 1 || env->key_ref[0] != 0)) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Unknown key referenced.\n");
		return SC_ERROR_NOT_SUPPORTED;
	}

	 
	if (env->flags & SC_ALGORITHM_RSA_PAD_PKCS1)
		priv->sec_padding = 0;
	else if (env->flags & SC_ALGORITHM_RSA_PAD_ANSI)
		priv->sec_padding = 1;
	else if (env->flags & SC_ALGORITHM_RSA_PAD_ISO9796)
		priv->sec_padding = 2;
	else {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Padding algorithm not supported.\n");
		return SC_ERROR_NOT_SUPPORTED;
	}

	switch (env->operation) {
	case SC_SEC_OPERATION_SIGN:
		 
		if (env->algorithm_flags & SC_ALGORITHM_RSA_HASH_SHA1) {
			context = GPK_SIGN_RSA_SHA;
			priv->sec_hash_len = 20;
		} else
		if (env->algorithm_flags & SC_ALGORITHM_RSA_HASH_MD5_SHA1) {
			context = GPK_SIGN_RSA_SSL;
			priv->sec_hash_len = 36;
		} else
		if (env->algorithm_flags & SC_ALGORITHM_RSA_HASH_MD5) {
			context = GPK_SIGN_RSA_MD5;
			priv->sec_hash_len = 16;
		} else {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Unsupported signature algorithm");
			return SC_ERROR_NOT_SUPPORTED;
		}
		break;
	case SC_SEC_OPERATION_DECIPHER:
		context = GPK_UNWRAP_RSA;
		break;
	default:
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Crypto operation not supported.\n");
		return SC_ERROR_NOT_SUPPORTED;
	}

	 
	if (env->flags & SC_SEC_ENV_FILE_REF_PRESENT) {
		if (env->file_ref.len != 2) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "File reference: invalid length.\n");
			return SC_ERROR_INVALID_ARGUMENTS;
		}
		file_id = (env->file_ref.value[0] << 8)
			| env->file_ref.value[1];
	} else {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "File reference missing.\n");
		return SC_ERROR_INVALID_ARGUMENTS;
	}

	 
	r = gpk_select_id(card, GPK_SEL_EF, file_id, NULL);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Failed to select PK file");

	 
	r = sc_read_record(card, 1, sysrec, sizeof(sysrec),
			SC_RECORD_BY_REC_NR);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Failed to read PK sysrec");
	if (r != 7 || sysrec[0] != 0) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "First record of file is not the sysrec");
		return SC_ERROR_OBJECT_NOT_VALID;
	}
	if (sysrec[5] != 0x00) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Public key is not an RSA key");
		return SC_ERROR_OBJECT_NOT_VALID;
	}
	switch (sysrec[1]) {
	case 0x00: priv->sec_mod_len =  512 / 8; break;
	case 0x10: priv->sec_mod_len =  768 / 8; break;
	case 0x11: priv->sec_mod_len = 1024 / 8; break;
	default:
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Unsupported modulus length");
		return SC_ERROR_OBJECT_NOT_VALID;
	}

	 
	memset(&apdu, 0, sizeof(apdu));
	apdu.cse = SC_APDU_CASE_1;
	apdu.cla = 0x80;
	apdu.ins = 0xA6;
	apdu.p1  = file_id & 0x1f;
	apdu.p2  = context;

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	return r;
}