ReadUserLogState::ScoreFile( const char *path, int rot ) const
{
	StatStructType	statbuf;

	if ( NULL == path ) {
		path = CurPath( );
	}
	if ( rot < 0 ) {
		rot = m_cur_rot;
	}
	if ( StatFile( path, statbuf ) ) {
		dprintf( D_FULLDEBUG, "ScoreFile: stat Error\n" );
		return -1;
	}

	return ScoreFile( statbuf, rot );
}
