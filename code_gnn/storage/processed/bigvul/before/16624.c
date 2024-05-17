ReadUserLogState::GetStateString( MyString &str, const char *label ) const
{
	str = "";
	if ( NULL != label ) {
		str.sprintf( "%s:\n", label );
	}
	str.sprintf_cat (
		"  BasePath = %s\n"
		"  CurPath = %s\n"
		"  UniqId = %s, seq = %d\n"
		"  rotation = %d; max = %d; offset = %ld; event = %ld; type = %d\n"
		"  inode = %u; ctime = %d; size = %ld\n",
		m_base_path.Value(), m_cur_path.Value(),
		m_uniq_id.Value(), m_sequence,
		m_cur_rot, m_max_rotations, (long) m_offset,
		(long) m_event_num, m_log_type,
		(unsigned)m_stat_buf.st_ino, (int)m_stat_buf.st_ctime,
		(long)m_stat_buf.st_size );
}
