static void echo_erase_tab(unsigned int num_chars, int after_tab,
			   struct n_tty_data *ldata)
{
	add_echo_byte(ECHO_OP_START, ldata);
	add_echo_byte(ECHO_OP_ERASE_TAB, ldata);

	 
	num_chars &= 7;

	 
	if (after_tab)
		num_chars |= 0x80;

	add_echo_byte(num_chars, ldata);
}
