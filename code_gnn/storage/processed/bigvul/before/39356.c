static int fdc_dtr(void)
{
	 
	if ((raw_cmd->rate & 3) == FDCS->dtr)
		return 0;

	 
	fd_outb(raw_cmd->rate & 3, FD_DCR);

	 
	FDCS->dtr = raw_cmd->rate & 3;
	return fd_wait_for_completion(jiffies + 2UL * HZ / 100, floppy_ready);
}				 
