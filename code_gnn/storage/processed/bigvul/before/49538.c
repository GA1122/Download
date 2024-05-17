static inline int do_tuner_callback(struct dvb_frontend *fe, int cmd, int arg)
{
	struct xc2028_data *priv = fe->tuner_priv;

	 

	return (!fe->callback) ? -EINVAL :
		fe->callback(((fe->dvb) && (fe->dvb->priv)) ?
				fe->dvb->priv : priv->i2c_props.adap->algo_data,
			     DVB_FRONTEND_COMPONENT_TUNER, cmd, arg);
}
