static int generic_set_freq(struct dvb_frontend *fe, u32 freq  ,
			    enum v4l2_tuner_type new_type,
			    unsigned int type,
			    v4l2_std_id std,
			    u16 int_freq)
{
	struct xc2028_data *priv = fe->tuner_priv;
	int		   rc = -EINVAL;
	unsigned char	   buf[4];
	u32		   div, offset = 0;

	tuner_dbg("%s called\n", __func__);

	mutex_lock(&priv->lock);

	tuner_dbg("should set frequency %d kHz\n", freq / 1000);

	if (check_firmware(fe, type, std, int_freq) < 0)
		goto ret;

	 
	switch (new_type) {
	case V4L2_TUNER_ANALOG_TV:
		rc = send_seq(priv, {0x00, 0x00});

		 
		break;
	case V4L2_TUNER_RADIO:
		 
		break;
	case V4L2_TUNER_DIGITAL_TV:
		 

		 

		 

		if (priv->cur_fw.type & DTV6)
			offset = 1750000;
		else	 
			offset = 2750000;

		 

		 

#if 0
		 

		if (priv->firm_version < 0x0302) {
			if (priv->cur_fw.type & DTV7)
				offset += 500000;
		} else {
			if (priv->cur_fw.type & DTV7)
				offset -= 300000;
			else if (type != ATSC)  
				offset += 200000;
		}
#endif
		break;
	default:
		tuner_err("Unsupported tuner type %d.\n", new_type);
		break;
	}

	div = (freq - offset + DIV / 2) / DIV;

	 
	if (priv->firm_version < 0x0202)
		rc = send_seq(priv, {0x00, XREG_RF_FREQ, 0x00, 0x00});
	else
		rc = send_seq(priv, {0x80, XREG_RF_FREQ, 0x00, 0x00});
	if (rc < 0)
		goto ret;

	 
	if (priv->ctrl.msleep)
		msleep(priv->ctrl.msleep);
	do_tuner_callback(fe, XC2028_RESET_CLK, 1);

	msleep(10);

	buf[0] = 0xff & (div >> 24);
	buf[1] = 0xff & (div >> 16);
	buf[2] = 0xff & (div >> 8);
	buf[3] = 0xff & (div);

	rc = i2c_send(priv, buf, sizeof(buf));
	if (rc < 0)
		goto ret;
	msleep(100);

	priv->frequency = freq;

	tuner_dbg("divisor= %*ph (freq=%d.%03d)\n", 4, buf,
	       freq / 1000000, (freq % 1000000) / 1000);

	rc = 0;

ret:
	mutex_unlock(&priv->lock);

	return rc;
}
