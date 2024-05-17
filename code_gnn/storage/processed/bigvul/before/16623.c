ReadUserLogState::GetState( ReadUserLog::FileState &state ) const
{
	ReadUserLogFileState			 fstate( state );
	ReadUserLogFileState::FileState *istate = fstate.getRwState();
	if ( !istate ) {
		return false;
	}

	if ( strcmp( istate->m_signature, FileStateSignature ) ) {
		return false;
	}
	if ( istate->m_version != FILESTATE_VERSION ) {
		return false;
	}

	if( !strlen( istate->m_base_path ) ) {
		memset( istate->m_base_path, 0, sizeof(istate->m_base_path) );
		if ( m_base_path.Value() ) {
			strncpy( istate->m_base_path,
					 m_base_path.Value(),
					 sizeof(istate->m_base_path) - 1 );
		}
	}


	istate->m_rotation = m_cur_rot;

	istate->m_log_type = m_log_type;

	if( m_uniq_id.Value() ) {
		strncpy( istate->m_uniq_id,
				 m_uniq_id.Value(),
				 sizeof(istate->m_uniq_id) );
		istate->m_uniq_id[sizeof(istate->m_uniq_id) - 1] = '\0';
	}
	else {
		memset( istate->m_uniq_id, 0, sizeof(istate->m_uniq_id) );
	}
	istate->m_sequence      = m_sequence;
	istate->m_max_rotations = m_max_rotations;

	istate->m_inode         = m_stat_buf.st_ino;
	istate->m_ctime         = m_stat_buf.st_ctime;
	istate->m_size.asint    = m_stat_buf.st_size;

	istate->m_offset.asint  = m_offset;
	istate->m_event_num.asint = m_event_num;

	istate->m_log_position.asint = m_log_position;
	istate->m_log_record.asint   = m_log_record;

	istate->m_update_time = m_update_time;

	return true;
}
