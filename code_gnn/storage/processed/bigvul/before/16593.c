FileTransfer::RemoveInputFiles(const char *sandbox_path)
{
	char *old_iwd;
	int old_transfer_flag;
	StringList do_not_remove;
	const char *f;

	if (!sandbox_path) {
		ASSERT(SpoolSpace);
		sandbox_path = SpoolSpace;
	}

	if ( !IsDirectory(sandbox_path) ) {
		return;
	}

	old_iwd = Iwd;
	old_transfer_flag = m_final_transfer_flag;

	Iwd = strdup(sandbox_path);
	m_final_transfer_flag = 1;

	ComputeFilesToSend();

	if ( FilesToSend == NULL ) {
		FilesToSend = OutputFiles;
		EncryptFiles = EncryptOutputFiles;
		DontEncryptFiles = DontEncryptOutputFiles;
	}

	FilesToSend->rewind();
	while ( (f=FilesToSend->next()) ) {
		do_not_remove.append( condor_basename(f) );
	}

	Directory dir( sandbox_path, desired_priv_state );

	while( (f=dir.Next()) ) {
		if( dir.IsDirectory() ) {
			continue;
		}
			
		if ( do_not_remove.file_contains(f) == TRUE ) {
			continue;
		}

		dir.Remove_Current_File();
	}

	m_final_transfer_flag = old_transfer_flag;
	free(Iwd);
	Iwd = old_iwd;

	return;
}
