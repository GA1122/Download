init_tilde()
{
	if( tilde ) {
		free( tilde );
		tilde = NULL;
	}
# if defined UNIX
	struct passwd *pw;
	if( (pw=getpwnam( myDistro->Get() )) ) {
		tilde = strdup( pw->pw_dir );
	}
# else
	HKEY	handle;
	char regKey[1024];

	snprintf( regKey, 1024, "Software\\%s", myDistro->GetCap() );

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, regKey,
		0, KEY_READ, &handle) == ERROR_SUCCESS ) {


		char the_path[MAX_PATH];
		DWORD valType;
		DWORD valSize = MAX_PATH - 2;

		the_path[0] = '\0';

		if ( RegQueryValueEx(handle, "TILDE", 0,
			&valType, (unsigned char *)the_path, &valSize) == ERROR_SUCCESS ) {

			if ( valType == REG_SZ && the_path[0] ) {
				tilde = strdup(the_path);
			}
		}
		RegCloseKey(handle);
	}
	
# endif
}
