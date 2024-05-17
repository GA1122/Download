static int canon_copy_from_read_buf(struct tty_struct *tty,
				    unsigned char __user **b,
				    size_t *nr)
{
	struct n_tty_data *ldata = tty->disc_data;
	size_t n, size, more, c;
	size_t eol;
	size_t tail;
	int ret, found = 0;

	 
	if (!*nr)
		return 0;

	n = min(*nr + 1, smp_load_acquire(&ldata->canon_head) - ldata->read_tail);

	tail = ldata->read_tail & (N_TTY_BUF_SIZE - 1);
	size = min_t(size_t, tail + n, N_TTY_BUF_SIZE);

	n_tty_trace("%s: nr:%zu tail:%zu n:%zu size:%zu\n",
		    __func__, *nr, tail, n, size);

	eol = find_next_bit(ldata->read_flags, size, tail);
	more = n - (size - tail);
	if (eol == N_TTY_BUF_SIZE && more) {
		 
		eol = find_next_bit(ldata->read_flags, more, 0);
		found = eol != more;
	} else
		found = eol != size;

	n = eol - tail;
	if (n > N_TTY_BUF_SIZE)
		n += N_TTY_BUF_SIZE;
	c = n + found;

	if (!found || read_buf(ldata, eol) != __DISABLED_CHAR) {
		c = min(*nr, c);
		n = c;
	}

	n_tty_trace("%s: eol:%zu found:%d n:%zu c:%zu tail:%zu more:%zu\n",
		    __func__, eol, found, n, c, tail, more);

	ret = tty_copy_to_user(tty, *b, tail, n);
	if (ret)
		return -EFAULT;
	*b += n;
	*nr -= n;

	if (found)
		clear_bit(eol, ldata->read_flags);
	smp_store_release(&ldata->read_tail, ldata->read_tail + c);

	if (found) {
		if (!ldata->push)
			ldata->line_start = ldata->read_tail;
		else
			ldata->push = 0;
		tty_audit_push();
	}
	return 0;
}