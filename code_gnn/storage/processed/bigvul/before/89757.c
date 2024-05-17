static void enforce_filters(void) {
	arg_nonewprivs = 1;
	force_nonewprivs = 1;

	fmessage("\n**     Warning: dropping all Linux capabilities     **\n");
	arg_caps_drop_all = 1;

	arg_nogroups = 1;
}
