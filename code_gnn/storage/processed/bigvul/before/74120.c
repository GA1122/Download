static void get_login_defs(struct chfn_control *ctl)
{
	const char *s;
	size_t i;
	int broken = 0;

	 
	if (geteuid() == getuid() && getuid() == 0) {
		ctl->allow_fullname = ctl->allow_room = ctl->allow_work = ctl->allow_home = 1;
		return;
	}
	s = getlogindefs_str("CHFN_RESTRICT", "");
	if (!strcmp(s, "yes")) {
		ctl->allow_room = ctl->allow_work = ctl->allow_home = 1;
		return;
	}
	if (!strcmp(s, "no")) {
		ctl->allow_fullname = ctl->allow_room = ctl->allow_work = ctl->allow_home = 1;
		return;
	}
	for (i = 0; s[i]; i++) {
		switch (s[i]) {
		case 'f':
			ctl->allow_fullname = 1;
			break;
		case 'r':
			ctl->allow_room = 1;
			break;
		case 'w':
			ctl->allow_work = 1;
			break;
		case 'h':
			ctl->allow_home = 1;
			break;
		default:
			broken = 1;
		}
	}
	if (broken)
		warnx(_("%s: CHFN_RESTRICT has unexpected value: %s"), _PATH_LOGINDEFS, s);
	if (!ctl->allow_fullname && !ctl->allow_room && !ctl->allow_work && !ctl->allow_home)
		errx(EXIT_FAILURE, _("%s: CHFN_RESTRICT does not allow any changes"), _PATH_LOGINDEFS);
	return;
}