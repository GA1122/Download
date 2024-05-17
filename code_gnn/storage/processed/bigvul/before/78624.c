static int piv_card_reader_lock_obtained(sc_card_t *card, int was_reset)
{
	int r = 0;
	u8 temp[256];
	size_t templen = sizeof(temp);
	piv_private_data_t * priv = PIV_DATA(card);  

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 
	if (priv == NULL || priv->pstate == PIV_STATE_MATCH) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,
				priv ? "PIV_STATE_MATCH" : "priv==NULL");
		r = 0;  
		goto err;
	}

	 

	 
	 

	 
	if (priv->card_issues & CI_DISCOVERY_USELESS) {
	    r =  SC_ERROR_NO_CARD_SUPPORT;
	} else {
	    r = piv_find_discovery(card);
	}

	if (r < 0) {
		if (was_reset > 0 || !(priv->card_issues & CI_PIV_AID_LOSE_STATE)) {
			r = piv_select_aid(card, piv_aids[0].value, piv_aids[0].len_short, temp, &templen);
		} else {
			r = 0;  
		}
	}

	if (r < 0)  
		goto err;
	
	if (was_reset > 0)
		priv->logged_in =  SC_PIN_STATE_UNKNOWN;

	r = 0;

err:
	LOG_FUNC_RETURN(card->ctx, r);
}
