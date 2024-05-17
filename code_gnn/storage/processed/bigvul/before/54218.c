static int acm_tty_write_room(struct tty_struct *tty)
{
	struct acm *acm = tty->driver_data;
	 
	return acm_wb_is_avail(acm) ? acm->writesize : 0;
}
