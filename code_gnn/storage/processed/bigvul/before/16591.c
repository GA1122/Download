FileTransfer::Reaper(Service *, int pid, int exit_status)
{
	FileTransfer *transobject;
	bool read_failed = false;
	if ( TransThreadTable->lookup(pid,transobject) < 0) {
		dprintf(D_ALWAYS, "unknown pid %d in FileTransfer::Reaper!\n", pid);
		return FALSE;
	}
	transobject->ActiveTransferTid = -1;
	TransThreadTable->remove(pid);

	transobject->Info.duration = time(NULL)-transobject->TransferStart;
	transobject->Info.in_progress = false;
	if( WIFSIGNALED(exit_status) ) {
		transobject->Info.success = false;
		transobject->Info.try_again = true;
		transobject->Info.error_desc.sprintf("File transfer failed (killed by signal=%d)", WTERMSIG(exit_status));
		read_failed = true;  
		dprintf( D_ALWAYS, "%s\n", transobject->Info.error_desc.Value() );
	} else {
		if( WEXITSTATUS(exit_status) != 0 ) {
			dprintf( D_ALWAYS, "File transfer completed successfully.\n" );
			transobject->Info.success = true;
		} else {
			dprintf( D_ALWAYS, "File transfer failed (status=%d).\n",
					 WEXITSTATUS(exit_status) );
			transobject->Info.success = false;
		}
	}

	if( transobject->TransferPipe[1] != -1 ) {
		close(transobject->TransferPipe[1]);
		transobject->TransferPipe[1] = -1;
	}

	int n;

	if(!read_failed) {
		n = read( transobject->TransferPipe[0],
				  (char *)&transobject->Info.bytes,
				  sizeof( filesize_t) );
		if(n != sizeof( filesize_t )) read_failed = true;
	}

	if(!read_failed) {
		n = read( transobject->TransferPipe[0],
				  (char *)&transobject->Info.try_again,
				  sizeof( bool ) );
		if(n != sizeof( bool )) read_failed = true;
	}

	
	if(!read_failed) {
		n = read( transobject->TransferPipe[0],
				  (char *)&transobject->Info.hold_code,
				  sizeof( int ) );
		if(n != sizeof( int )) read_failed = true;
	}

	if(!read_failed) {
		n = read( transobject->TransferPipe[0],
				  (char *)&transobject->Info.hold_subcode,
				  sizeof( int ) );
		if(n != sizeof( int )) read_failed = true;
	}

	int error_len = 0;
	if(!read_failed) {
		n = read( transobject->TransferPipe[0],
				  (char *)&error_len,
				  sizeof( int ) );
		if(n != sizeof( int )) read_failed = true;
	}

	if(!read_failed && error_len) {
		char *error_buf = new char[error_len];
		ASSERT(error_buf);

		n = read( transobject->TransferPipe[0],
			  error_buf,
			  error_len );
		if(n != error_len) read_failed = true;
		if(!read_failed) {
			transobject->Info.error_desc = error_buf;
		}

		delete [] error_buf;
	}

	int spooled_files_len = 0;
	if(!read_failed) {
		n = read( transobject->TransferPipe[0],
				  (char *)&spooled_files_len,
				  sizeof( int ) );
		if(n != sizeof( int )) read_failed = true;
	}

	if(!read_failed && spooled_files_len) {
		char *spooled_files_buf = new char[spooled_files_len];
		ASSERT(spooled_files_buf);

		n = read( transobject->TransferPipe[0],
			  spooled_files_buf,
			  spooled_files_len );
		if(n != spooled_files_len) read_failed = true;
		if(!read_failed) {
			transobject->Info.spooled_files = spooled_files_buf;
		}

		delete [] spooled_files_buf;
	}

	if(read_failed) {
		transobject->Info.success = false;
		transobject->Info.try_again = true;
		if( transobject->Info.error_desc.IsEmpty() ) {
			transobject->Info.error_desc.sprintf("Failed to read status report from file transfer pipe (errno %d): %s",errno,strerror(errno));
			dprintf(D_ALWAYS,"%s\n",transobject->Info.error_desc.Value());
		}
	}

	close(transobject->TransferPipe[0]);
	transobject->TransferPipe[0] = -1;

	if ( transobject->Info.success && transobject->upload_changed_files &&
		 transobject->IsClient() && transobject->Info.type == DownloadFilesType ) {
		time(&(transobject->last_download_time));
		transobject->BuildFileCatalog(0, transobject->Iwd, &(transobject->last_download_catalog));
		sleep(1);
	}

	if (transobject->ClientCallback) {
		dprintf(D_FULLDEBUG,
				"Calling client FileTransfer handler function.\n");
		((transobject->ClientCallbackClass)->*(transobject->ClientCallback))(transobject);
	}

	return TRUE;
}
