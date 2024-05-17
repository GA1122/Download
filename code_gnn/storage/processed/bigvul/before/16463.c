check_log_dir()
{
	const char	*f;
	Directory dir(Log, PRIV_ROOT);
	StringList invalid;

	invalid.initializeFromString (InvalidLogFiles ? InvalidLogFiles : "");

	while( (f = dir.Next()) ) {
		if( invalid.contains(f) ) {
			bad_file( Log, f, dir );
		} else {
			good_file( Log, f );
		}
	}
}
