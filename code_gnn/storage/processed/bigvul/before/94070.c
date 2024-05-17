static int cx24116_tune(struct dvb_frontend *fe, bool re_tune,
	unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	 

	*delay = HZ / 5;
	if (re_tune) {
		int ret = cx24116_set_frontend(fe);
		if (ret)
			return ret;
	}
	return cx24116_read_status(fe, status);
}
