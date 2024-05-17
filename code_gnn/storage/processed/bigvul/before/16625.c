ReadUserLogState::GetStateString(
	const ReadUserLog::FileState	&state,
	MyString						&str,
	const char						*label
  ) const
{
	const ReadUserLogFileState::FileState *istate;
	if ( ( !convertState(state, istate) ) || ( !istate->m_version ) ) {
		if ( label ) {
			str.sprintf( "%s: no state", label );
		}
		else {
			str = "no state\n";
		}
		return;
	}

	str = "";
	if ( NULL != label ) {
		str.sprintf( "%s:\n", label );
	}
	str.sprintf_cat (
		"  signature = '%s'; version = %d; update = %ld\n"
		"  base path = '%s'\n"
		"  cur path = '%s'\n"
		"  UniqId = %s, seq = %d\n"
		"  rotation = %d; max = %d; offset = "FILESIZE_T_FORMAT";"
		" event num = "FILESIZE_T_FORMAT"; type = %d\n"
		"  inode = %u; ctime = %ld; size = "FILESIZE_T_FORMAT"\n",
		istate->m_signature, istate->m_version, istate->m_update_time,
		istate->m_base_path,
		CurPath(state),
		istate->m_uniq_id, istate->m_sequence,
		istate->m_rotation, istate->m_max_rotations,
		istate->m_offset.asint,
		istate->m_event_num.asint,
		istate->m_log_type,
		(unsigned)istate->m_inode, istate->m_ctime, istate->m_size.asint );
}
