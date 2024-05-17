ReadUserLogState::ScoreFile( const StatStructType &statbuf, int rot ) const
{
	int		score = 0;

	if ( rot < 0 ) {
		rot = m_cur_rot;
	}

	bool	is_recent = ( time(NULL) < (m_update_time + m_recent_thresh) );
	bool	is_current = ( rot == m_cur_rot );
	bool	same_size = ( statbuf.st_size == m_stat_buf.st_size );
	bool 	has_grown = ( statbuf.st_size > m_stat_buf.st_size );

	MyString	MatchList = "";	 

	if ( m_stat_buf.st_ino == statbuf.st_ino ) {
		score += m_score_fact_inode;
		if ( DebugFlags & D_FULLDEBUG ) MatchList += "inode ";
	}

	if ( m_stat_buf.st_ctime == statbuf.st_ctime ) {
		score += m_score_fact_ctime;
		if ( DebugFlags & D_FULLDEBUG ) MatchList += "ctime ";
	}

	if ( same_size ) {
		score += m_score_fact_same_size;
		if ( DebugFlags & D_FULLDEBUG ) MatchList += "same-size ";
	}
	else if ( is_recent && is_current && has_grown ) {
		score += m_score_fact_grown;
		if ( DebugFlags & D_FULLDEBUG ) MatchList += "grown ";
	}

	if ( m_stat_buf.st_size > statbuf.st_size ) {
		score += m_score_fact_shrunk;
		if ( DebugFlags & D_FULLDEBUG ) MatchList += "shrunk ";
	}

	if ( DebugFlags & D_FULLDEBUG ) {
		dprintf( D_FULLDEBUG, "ScoreFile: match list: %s\n",
				 MatchList.Value() );
	}

	if ( score < 0 ) {
		score = 0;
	}

	return score;
}
