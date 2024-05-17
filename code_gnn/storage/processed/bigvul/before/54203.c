static int acm_tty_chars_in_buffer(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;
	 
	if (acm->disconnected)
		return 0;
	 
	return (ACM_NW - acm_wb_is_avail(acm)) * acm->writesize;
}
