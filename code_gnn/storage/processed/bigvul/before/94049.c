static int cx24116_diseqc_send_burst(struct dvb_frontend *fe,
	fe_sec_mini_cmd_t burst)
{
	struct cx24116_state *state = fe->demodulator_priv;
	int ret;

	dprintk("%s(%d) toneburst=%d\n", __func__, burst, toneburst);

	 
	if (burst == SEC_MINI_A)
		state->dsec_cmd.args[CX24116_DISEQC_BURST] =
			CX24116_DISEQC_MINI_A;
	else if (burst == SEC_MINI_B)
		state->dsec_cmd.args[CX24116_DISEQC_BURST] =
			CX24116_DISEQC_MINI_B;
	else
		return -EINVAL;

	 
	if (toneburst != CX24116_DISEQC_MESGCACHE)
		 
		return 0;

	 

	 
	ret = cx24116_wait_for_lnb(fe);
	if (ret != 0)
		return ret;

	 
	msleep(100);

	 
	ret = cx24116_cmd_execute(fe, &state->dsec_cmd);
	if (ret != 0)
		return ret;

	 
	msleep((state->dsec_cmd.args[CX24116_DISEQC_MSGLEN] << 4) + 60);

	return 0;
}
