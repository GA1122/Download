static void isig(int sig, struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	if (L_NOFLSH(tty)) {
		 
		__isig(sig, tty);

	} else {  
		up_read(&tty->termios_rwsem);
		down_write(&tty->termios_rwsem);

		__isig(sig, tty);

		 
		mutex_lock(&ldata->output_lock);
		ldata->echo_head = ldata->echo_tail = 0;
		ldata->echo_mark = ldata->echo_commit = 0;
		mutex_unlock(&ldata->output_lock);

		 
		tty_driver_flush_buffer(tty);

		 
		reset_buffer_flags(tty->disc_data);

		 
		if (tty->link)
			n_tty_packet_mode_flush(tty);

		up_write(&tty->termios_rwsem);
		down_read(&tty->termios_rwsem);
	}
}