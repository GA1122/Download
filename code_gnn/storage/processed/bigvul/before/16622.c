ReadUserLogState::GeneratePath( int rotation,
								MyString &path,
								bool initializing ) const
{
	if ( !initializing && !m_initialized ) {
		return false;
	}

	if (  ( rotation < 0 ) || ( rotation > m_max_rotations )  ) {
		return false;
	}

	if ( !m_base_path.Length() ) {
		path = "";
		return false;
	}

	path = m_base_path;
	if ( rotation ) {
		if ( m_max_rotations > 1 ) {
			path.sprintf_cat( ".%d", rotation );
		}
		else {
			path += ".old";
		}
	}

	return true;
}
