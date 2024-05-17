const char *tty_name(const struct tty_struct *tty)
{
	if (!tty)  
		return "NULL tty";
	return tty->name;
}
