piv_pin_cmd(sc_card_t *card, struct sc_pin_cmd_data *data, int *tries_left)
{
	int r = 0;
	piv_private_data_t * priv = PIV_DATA(card);

	 
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	sc_log(card->ctx, "piv_pin_cmd tries_left=%d, logged_in=%d", priv->tries_left, priv->logged_in);
	if (data->cmd == SC_PIN_CMD_CHANGE) {
		int i = 0;
		if (data->pin2.len < 6) {
			return SC_ERROR_INVALID_PIN_LENGTH;
		}
		for(i=0; i < data->pin2.len; ++i) {
			if (!isdigit(data->pin2.data[i])) {
				return SC_ERROR_INVALID_DATA;
			}
		}
	}

	priv->pin_cmd_verify_sw1 = 0x00U;

	if (data->cmd == SC_PIN_CMD_GET_INFO) {  
		data->pin1.logged_in = priv->logged_in;
		data->pin1.tries_left = priv->tries_left;
		if (tries_left)
			*tries_left = priv->tries_left;

		 
		if (data->pin_type == SC_AC_CONTEXT_SPECIFIC) {
			data->pin1.logged_in = 0;
			 LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
		}

		if (priv->logged_in == SC_PIN_STATE_LOGGED_IN) {
			 
			LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
		}
	}

	 
	if (data->cmd == SC_PIN_CMD_VERIFY && data->pin_type == SC_AC_CONTEXT_SPECIFIC) {
		priv->context_specific = 1;
		sc_log(card->ctx,"Starting CONTEXT_SPECIFIC verify");
		sc_lock(card);
	}

	priv->pin_cmd_verify = 1;  
	r = iso_drv->ops->pin_cmd(card, data, tries_left);
	priv->pin_cmd_verify = 0;

	 
	if (data->cmd == SC_PIN_CMD_VERIFY && r < 0 &&  priv->context_specific) {
		sc_log(card->ctx,"Clearing CONTEXT_SPECIFIC");
		priv->context_specific = 0;
		sc_unlock(card);
	}

	 
	if ( priv->card_issues & CI_OTHER_AID_LOSE_STATE && priv->pin_cmd_verify_sw1 == 0x6DU) {
		sc_log(card->ctx, "AID may be lost doing piv_find_aid and retry pin_cmd");
		piv_find_aid(card, priv->aid_file);  

		priv->pin_cmd_verify = 1;  
		r = iso_drv->ops->pin_cmd(card, data, tries_left);
		priv->pin_cmd_verify = 0;
	}

	 
	if (data->cmd == SC_PIN_CMD_VERIFY) {
	    if (r >= 0)
		priv->logged_in = SC_PIN_STATE_LOGGED_IN;
	    else
		priv->logged_in = SC_PIN_STATE_LOGGED_OUT;
	}

	 
	 

	if (data->cmd == SC_PIN_CMD_GET_INFO) {
		if (priv->card_issues & CI_CANT_USE_GETDATA_FOR_STATE) {
			sc_log(card->ctx, "CI_CANT_USE_GETDATA_FOR_STATE set, assume logged_in=%d", priv->logged_in);
			data->pin1.logged_in =  priv->logged_in;  
		} else if (priv->card_issues & CI_VERIFY_LC0_FAIL
			&& priv->pin_cmd_verify_sw1 == 0x63U ) {  
				 
			    r = piv_check_protected_objects(card);
			if (r == SC_SUCCESS)
				data->pin1.logged_in = SC_PIN_STATE_LOGGED_IN;
			else if (r ==  SC_ERROR_PIN_CODE_INCORRECT) {
				if (priv->card_issues & CI_CANT_USE_GETDATA_FOR_STATE) {  

					data->pin1.logged_in = priv->logged_in;  
					 
				} else {
					data->pin1.logged_in = SC_PIN_STATE_LOGGED_OUT;
				}
				r = SC_SUCCESS;
			}
		}
		priv->logged_in = data->pin1.logged_in;
		priv->tries_left = data->pin1.tries_left;
	}

	sc_log(card->ctx, "piv_pin_cmd tries_left=%d, logged_in=%d",priv->tries_left, priv->logged_in);
	LOG_FUNC_RETURN(card->ctx, r);
}
