ReadUserLogState::StatFile( const char *path, StatStructType &statbuf ) const
{
	StatWrapper	statwrap;
	if ( statwrap.Stat( path, StatWrapper::STATOP_STAT ) ) {
		return statwrap.GetRc( );
	}

	statwrap.GetBuf( statbuf );

	return 0;
}
