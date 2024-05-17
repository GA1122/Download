static int tt_s2_4600_read_status(struct dvb_frontend *fe,
				  enum fe_status *status)
{
	struct dvb_usb_adapter *d =
		(struct dvb_usb_adapter *)(fe->dvb->priv);
	struct dw2102_state *st = (struct dw2102_state *)d->dev->priv;
	int ret;

	ret = st->fe_read_status(fe, status);

	 
	if ((*status & FE_HAS_LOCK) && (!st->last_lock))
		su3000_streaming_ctrl(d, 1);

	st->last_lock = (*status & FE_HAS_LOCK) ? 1 : 0;
	return ret;
}