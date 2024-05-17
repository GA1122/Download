ReadUserLogState::CurPath( const ReadUserLog::FileState &state ) const
{
	const ReadUserLogFileState::FileState *istate;
	if ( ( !convertState(state, istate) ) || ( !istate->m_version ) ) {
		return NULL;
	}

	static MyString	path;
	if ( !GeneratePath( istate->m_rotation, path, true ) ) {
		return NULL;
	}
	return path.Value();
}
