void smb_init_locale(void)
{
	 
#ifdef HAVE_SETLOCALE
	setlocale(LC_ALL, "");
#endif
}
