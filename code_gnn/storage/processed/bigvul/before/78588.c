pgp_get_card_features(sc_card_t *card)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	u8 *hist_bytes = card->reader->atr_info.hist_bytes;
	size_t hist_bytes_len = card->reader->atr_info.hist_bytes_len;
	size_t i;
	pgp_blob_t *blob, *blob6e, *blob73;

	 
	if (hist_bytes_len > 0) {
		 
		switch (hist_bytes[0]) {
			case 0x00:
				pgp_parse_hist_bytes(card, hist_bytes+1, hist_bytes_len-4);
				break;
			case 0x80:
				pgp_parse_hist_bytes(card, hist_bytes+1, hist_bytes_len-1);
				break;
			case 0x10:
				pgp_parse_hist_bytes(card, hist_bytes+2, hist_bytes_len-2);
				break;
		}
	}

	 
	priv->ext_caps &= ~EXT_CAP_LCS;

	if (priv->bcd_version >= OPENPGP_CARD_2_0) {
		 
		if ((pgp_get_blob(card, priv->mf, 0x5f52, &blob) >= 0) &&
		    (blob->data != NULL) && (blob->data[0] == 0x00)) {

			pgp_parse_hist_bytes(card, hist_bytes+1, hist_bytes_len-4);

			 
			if ((blob->data[0] == 0x00) && (blob->len >= 4)) {
				priv->state = blob->data[blob->len-3];
				 
				if (priv->state != CARD_STATE_UNKNOWN)
					priv->ext_caps |= EXT_CAP_LCS;
			}
		}
	}

	if (priv->bcd_version >= OPENPGP_CARD_3_1) {
		card->caps |= SC_CARD_CAP_ISO7816_PIN_INFO;
	}

	if ((pgp_get_blob(card, priv->mf, 0x006e, &blob6e) >= 0) &&
	    (pgp_get_blob(card, blob6e, 0x0073, &blob73) >= 0)) {

		 
		if ((pgp_get_blob(card, blob73, 0x00c0, &blob) >= 0) &&
		    (blob->data != NULL) && (blob->len > 0)) {
			 
			if ((blob->data[0] & 0x04) &&
					(priv->bcd_version >= OPENPGP_CARD_2_0))
				priv->ext_caps |= EXT_CAP_ALG_ATTR_CHANGEABLE;
			 
			if (blob->data[0] & 0x08)
				priv->ext_caps |= EXT_CAP_PRIVATE_DO;
			 
			if (blob->data[0] & 0x10)
				priv->ext_caps |= EXT_CAP_C4_CHANGEABLE;
			 
			if (blob->data[0] & 0x20)
				priv->ext_caps |= EXT_CAP_KEY_IMPORT;
			 
			if (blob->data[0] & 0x40) {
				card->caps |= SC_CARD_CAP_RNG;
				priv->ext_caps |= EXT_CAP_GET_CHALLENGE;
			}
			 
			if ((blob->data[0] & 0x80) &&
					(priv->bcd_version >= OPENPGP_CARD_2_0))
				priv->ext_caps |= EXT_CAP_SM;

			if ((priv->bcd_version >= OPENPGP_CARD_2_0) && (blob->len >= 10)) {
				 
				priv->max_challenge_size = bebytes2ushort(blob->data + 2);
				 
				priv->max_cert_size = bebytes2ushort(blob->data + 4);

				if (priv->bcd_version < OPENPGP_CARD_3_0) {
					 
					priv->sm_algo = blob->data[0];
					if ((priv->sm_algo == SM_ALGO_NONE) && (priv->ext_caps & EXT_CAP_SM))
						priv->sm_algo = SM_ALGO_3DES;

					 
					card->max_send_size = bebytes2ushort(blob->data + 6);
					card->max_recv_size = bebytes2ushort(blob->data + 8);
				}
				else {
					 
					priv->sm_algo = blob->data[0];
					if ((priv->sm_algo == SM_ALGO_NONE) && (priv->ext_caps & EXT_CAP_SM))
						priv->sm_algo = SM_ALGO_UNKNOWN;
				}
			}
		}

		 
		if ((pgp_get_blob(card, blob73, 0x00c4, &blob) >= 0) &&
			(blob->data != NULL) && (blob->len > 1)) {
			 
			card->max_pin_len = blob->data[1];
		}

		 
		for (i = 0x00c1; i <= 0x00c3; i++) {
			unsigned long flags;

			 
			 
			flags = SC_ALGORITHM_RSA_PAD_PKCS1;
			flags |= SC_ALGORITHM_RSA_HASH_NONE;
			 
			flags |= SC_ALGORITHM_ONBOARD_KEY_GEN;

			 
			if ((pgp_get_blob(card, blob73, i, &blob) >= 0) && (blob->data != NULL)) {
				if (blob->len >= 3 && blob->data[0] == 0x01) {	 
					unsigned int keybits = bebytes2ushort(blob->data + 1);

					_sc_card_add_rsa_alg(card, keybits, flags, 0);
				}
				 
			}
		}

		if (priv->bcd_version >= OPENPGP_CARD_3_0) {
			 
			if ((pgp_get_blob(card, blob6e, 0x7f66, &blob) >= 0) &&
				(blob->data != NULL) && (blob->len >= 8)) {
				 
				card->max_send_size = bebytes2ushort(blob->data + 2);
				card->max_recv_size = bebytes2ushort(blob->data + 6);
			}
		}
	}

	return SC_SUCCESS;
}
