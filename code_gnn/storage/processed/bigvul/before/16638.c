ReadUserLogState::Rotation( int rotation,
							StatStructType &statbuf,
							bool initializing )
{
	if ( !initializing && !m_initialized ) {
		return -1;
	}

	if (  ( rotation < 0 ) || ( rotation > m_max_rotations )  ) {
		return -1;
	}

	if ( m_cur_rot == rotation ) {
		return 0;
	}

	m_uniq_id = "";
	GeneratePath( rotation, m_cur_path, initializing );
	m_cur_rot = rotation;
	m_log_type = LOG_TYPE_UNKNOWN;
	Update();

	return StatFile( statbuf );
}
