read_filename (void)
{
	char fname[_POSIX_PATH_MAX];
	int r;

	fd_printf(STO, "\r\n*** file: ");
	r = fd_readline(STI, STO, fname, sizeof(fname));
	fd_printf(STO, "\r\n");
	if ( r < 0 ) 
		return NULL;
	else
		return strdup(fname);
}
