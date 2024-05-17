ReadUserLogState::Reset( ResetType type )
{
	if ( RESET_INIT == type ) {
		m_initialized = false;
		m_init_error = false;
		m_base_path = "";
		m_max_rotations = 0;

		m_recent_thresh = 0;
		m_score_fact_ctime = 0;
		m_score_fact_inode = 0;
		m_score_fact_same_size = 0;
		m_score_fact_grown = 0;
		m_score_fact_shrunk = 0;
	}

	else if ( RESET_FULL == type ) {
		m_base_path = "";
	}

	m_cur_path = "";
	m_cur_rot = -1;
	m_uniq_id = "";
	m_sequence = 0;

	memset( &m_stat_buf, 0, sizeof(m_stat_buf) );
	m_status_size = -1;

	m_stat_valid = false;
	m_stat_time = 0;

	m_log_position = 0;
	m_log_record = 0;

	m_offset = 0;
	m_event_num = 0;

	m_log_type = LOG_TYPE_UNKNOWN;

}
