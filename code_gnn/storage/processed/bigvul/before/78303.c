static int coolkey_initialize(sc_card_t *card)
{
	int r;
	coolkey_private_data_t *priv = NULL;
	coolkey_life_cycle_t life_cycle;
	coolkey_object_info_t object_info;
	int combined_processed = 0;

	 
	if (card->drv_data) {
		return SC_SUCCESS;
	}
	sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,"Coolkey Applet found");

	priv = coolkey_new_private_data();
	if (priv == NULL) {
		r = SC_ERROR_OUT_OF_MEMORY;
		goto cleanup;
	}
	r = coolkey_get_life_cycle(card, &life_cycle);
	if (r < 0) {
		goto cleanup;
	}

	 
	r = coolkey_select_applet(card);
	if (r < 0) {
		goto cleanup;
	}

	priv->protocol_version_major = life_cycle.protocol_version_major;
	priv->protocol_version_minor = life_cycle.protocol_version_minor;
	priv->pin_count = life_cycle.pin_count;
	priv->life_cycle = life_cycle.life_cycle;

	 
	for(r=coolkey_list_object(card, COOLKEY_LIST_RESET, &object_info); r >= 0;
		r= coolkey_list_object(card, COOLKEY_LIST_NEXT, &object_info)) {
		unsigned long object_id = bebytes2ulong(object_info.object_id);
		unsigned short object_len = bebytes2ulong(object_info.object_length);
	     


		 
		if (object_id == COOLKEY_COMBINED_OBJECT_ID) {
			u8 *object = malloc(object_len);
			if (object == NULL) {
				r = SC_ERROR_OUT_OF_MEMORY;
				break;
			}
			r = coolkey_read_object(card, COOLKEY_COMBINED_OBJECT_ID, 0, object, object_len,
											priv->nonce, sizeof(priv->nonce));
			if (r < 0) {
				free(object);
				break;
			}
			r = coolkey_process_combined_object(card, priv, object, r);
			free(object);
			if (r != SC_SUCCESS) {
				break;
			}
			combined_processed = 1;
			continue;
		}
		r = coolkey_add_object(priv, object_id, NULL, object_len, 0);
		if (r != SC_SUCCESS)
			sc_log(card->ctx, "coolkey_add_object() returned %d", r);

	}
	if (r != SC_ERROR_FILE_END_REACHED) {
		goto cleanup;
	}
	 
	if (!combined_processed) {
		global_platform_cplc_data_t cplc_data;
		 

		r = gp_select_card_manager(card);
		if (r < 0) {
			goto cleanup;
		}

		r = coolkey_get_cplc_data(card, &cplc_data);
		if (r < 0) {
			goto cleanup;
		}
		coolkey_make_cuid_from_cplc(&priv->cuid, &cplc_data);
		priv->token_name = (u8 *)strdup("COOLKEY");
		if (priv->token_name == NULL) {
			r= SC_ERROR_OUT_OF_MEMORY;
			goto cleanup;
		}
		priv->token_name_length = sizeof("COOLKEY")-1;
	}
	card->drv_data = priv;
	return SC_SUCCESS;

cleanup:
	if (priv) {
		coolkey_free_private_data(priv);
	}
	return r;
}
