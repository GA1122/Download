ac_to_acl(unsigned int ac, sc_file_t *file, unsigned int op)
{
	unsigned int	npins, pin;

	npins = (ac >> 14) & 3;
	if (npins == 3) {
		sc_file_add_acl_entry(file, op, SC_AC_NEVER,
			       	SC_AC_KEY_REF_NONE);
		return;
	}

	sc_file_add_acl_entry(file, op, SC_AC_NONE, SC_AC_KEY_REF_NONE);
	pin = ac & 0xFF;
	if (npins >= 1)
		sc_file_add_acl_entry(file, op, SC_AC_CHV, (pin >> 4) & 0xF);
	if (npins == 2)
		sc_file_add_acl_entry(file, op, SC_AC_CHV, pin & 0xF);

	 
	if (ac & 0x3F00)
		sc_file_add_acl_entry(file, op, SC_AC_PRO, (ac & 0x3F00) >> 8);
}