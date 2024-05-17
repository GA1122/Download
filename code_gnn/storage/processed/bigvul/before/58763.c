char *tty_name(struct tty_struct *tty, char *buf)
{
	if (!tty)  
		strcpy(buf, "NULL tty");
	else
		strcpy(buf, tty->name);
	return buf;
}
