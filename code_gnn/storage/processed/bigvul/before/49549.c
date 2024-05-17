static int xc2028_get_afc(struct dvb_frontend *fe, s32 *afc)
{
	struct xc2028_data *priv = fe->tuner_priv;
	int i, rc;
	u16 frq_lock = 0;
	s16 afc_reg = 0;

	rc = check_device_status(priv);
	if (rc < 0)
		return rc;

	 
	if (!rc) {
		*afc = 0;
		return 0;
	}

	mutex_lock(&priv->lock);

	 
	for (i = 0; i < 3; i++) {
		rc = xc2028_get_reg(priv, XREG_LOCK, &frq_lock);
		if (rc < 0)
			goto ret;

		if (frq_lock)
			break;
		msleep(6);
	}

	 
	if (frq_lock == 2)
		goto ret;

	 
	rc = xc2028_get_reg(priv, XREG_FREQ_ERROR, &afc_reg);
	if (rc < 0)
		goto ret;

	*afc = afc_reg * 15625;  

	tuner_dbg("AFC is %d Hz\n", *afc);

ret:
	mutex_unlock(&priv->lock);

	return rc;
}
