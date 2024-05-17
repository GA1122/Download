ReadUserLogState::ScoreFile( int rot ) const
{
	if ( rot > m_max_rotations ) {
		return -1;
	}
	else if ( rot < 0 ) {
		rot = m_cur_rot;
	}

	MyString	path;
	if ( !GeneratePath( rot, path ) ) {
		return -1;
	}
	return ScoreFile( path.Value(), rot );
}
