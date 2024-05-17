piv_check_protected_objects(sc_card_t *card)
{
	int r = 0;
	int i;
	piv_private_data_t * priv = PIV_DATA(card);
	u8 buf[8];  
	u8 * rbuf;
	size_t buf_len;
	static int protected_objects[] = {PIV_OBJ_PI, PIV_OBJ_CHF, PIV_OBJ_IRIS_IMAGE};

	LOG_FUNC_CALLED(card->ctx);
	 
	if (priv->object_test_verify == 0) {
		for (i = 0; i < (int)(sizeof(protected_objects)/sizeof(int)); i++) {
			buf_len = sizeof(buf);
			priv->pin_cmd_noparse = 1;  
			rbuf = buf;
			r = piv_get_data(card, protected_objects[i], &rbuf, &buf_len);
			priv->pin_cmd_noparse = 0;
			 
			if (r >= 0 || r == SC_ERROR_SECURITY_STATUS_NOT_SATISFIED) {

				 
				priv->object_test_verify = protected_objects[i];
				break;
			}
		}
		if (priv->object_test_verify == 0) {
			 
			sc_log(card->ctx, "No protected objects found, setting CI_CANT_USE_GETDATA_FOR_STATE");
			priv->card_issues |= CI_CANT_USE_GETDATA_FOR_STATE;
			r = SC_ERROR_PIN_CODE_INCORRECT;
		}
	} else {
		 
		buf_len = sizeof(buf);
		priv->pin_cmd_noparse = 1;  
		rbuf = buf;
		r = piv_get_data(card, priv->object_test_verify, &rbuf, &buf_len);
		priv->pin_cmd_noparse = 0;
	}
	if (r == SC_ERROR_FILE_NOT_FOUND)
		r = SC_ERROR_PIN_CODE_INCORRECT;
	else if (r == SC_ERROR_SECURITY_STATUS_NOT_SATISFIED)
		r = SC_ERROR_PIN_CODE_INCORRECT;
	else if (r > 0)
		r = SC_SUCCESS;

	sc_log(card->ctx, "object_test_verify=%d, card_issues = 0x%08x", priv->object_test_verify, priv->card_issues);
	LOG_FUNC_RETURN(card->ctx, r);
}
