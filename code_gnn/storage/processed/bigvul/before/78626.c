static int piv_check_sw(struct sc_card *card, unsigned int sw1, unsigned int sw2)
{
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();

	int r;
	piv_private_data_t * priv = PIV_DATA(card);

	 
	if (priv) {
		 
		if (priv->pin_cmd_verify) {
			priv->pin_cmd_verify_sw1 = sw1;
			priv->pin_cmd_verify_sw2 = sw2;
		} else {
			 
			 
			if (priv->context_specific) {
				sc_log(card->ctx,"Clearing CONTEXT_SPECIFIC lock");
				priv->context_specific = 0;
				sc_unlock(card);
			}
		}

		if (priv->card_issues & CI_VERIFY_630X) {

		 
			if (priv->pin_cmd_verify && sw1 == 0x63U) {
				priv->pin_cmd_verify_sw2 |= 0xC0U;  
				if ((sw2 & ~0x0fU) == 0x00U) {
					sc_log(card->ctx, "Verification failed (remaining tries: %d)", (sw2 & 0x0f));
					return SC_ERROR_PIN_CODE_INCORRECT;
					 
				}
			}
		}
	}
	r = iso_drv->ops->check_sw(card, sw1, sw2);
	return r;
}
