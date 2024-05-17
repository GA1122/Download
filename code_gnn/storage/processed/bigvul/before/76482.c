static int copy_from_read_buf(struct tty_struct *tty,
				      unsigned char __user **b,
				      size_t *nr)

{
	struct n_tty_data *ldata = tty->disc_data;
	int retval;
	size_t n;
	bool is_eof;
	size_t head = smp_load_acquire(&ldata->commit_head);
	size_t tail = ldata->read_tail & (N_TTY_BUF_SIZE - 1);

	retval = 0;
	n = min(head - ldata->read_tail, N_TTY_BUF_SIZE - tail);
	n = min(*nr, n);
	if (n) {
		const unsigned char *from = read_buf_addr(ldata, tail);
		retval = copy_to_user(*b, from, n);
		n -= retval;
		is_eof = n == 1 && *from == EOF_CHAR(tty);
		tty_audit_add_data(tty, from, n);
		smp_store_release(&ldata->read_tail, ldata->read_tail + n);
		 
		if (L_EXTPROC(tty) && ldata->icanon && is_eof &&
		    (head == ldata->read_tail))
			n = 0;
		*b += n;
		*nr -= n;
	}
	return retval;
}