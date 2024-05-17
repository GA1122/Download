static int receive_room(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;
	int left;

	if (I_PARMRK(tty)) {
		 
		left = N_TTY_BUF_SIZE - read_cnt(ldata) * 3 - 1;
	} else
		left = N_TTY_BUF_SIZE - read_cnt(ldata) - 1;

	 
	if (left <= 0)
		left = ldata->icanon && ldata->canon_head == ldata->read_tail;

	return left;
}
