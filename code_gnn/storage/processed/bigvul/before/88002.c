static int pf_command(struct pf_unit *pf, char *cmd, int dlen, char *fun)
{
	pi_connect(pf->pi);

	write_reg(pf, 6, 0xa0+0x10*pf->drive);

	if (pf_wait(pf, STAT_BUSY | STAT_DRQ, 0, fun, "before command")) {
		pi_disconnect(pf->pi);
		return -1;
	}

	write_reg(pf, 4, dlen % 256);
	write_reg(pf, 5, dlen / 256);
	write_reg(pf, 7, 0xa0);	 

	if (pf_wait(pf, STAT_BUSY, STAT_DRQ, fun, "command DRQ")) {
		pi_disconnect(pf->pi);
		return -1;
	}

	if (read_reg(pf, 2) != 1) {
		printk("%s: %s: command phase error\n", pf->name, fun);
		pi_disconnect(pf->pi);
		return -1;
	}

	pi_write_block(pf->pi, cmd, 12);

	return 0;
}