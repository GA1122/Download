config( int wantsQuiet, bool ignore_invalid_entry, bool wantsExtraInfo )
{
#ifdef WIN32
	char *locale = setlocale( LC_ALL, "English" );
	dprintf ( D_ALWAYS, "Locale: %s\n", locale );
#endif
	real_config( NULL, wantsQuiet, wantsExtraInfo );
	validate_entries( ignore_invalid_entry );
}
