FileTransfer::WriteStatusToTransferPipe(filesize_t total_bytes)
{
	int n;
	bool write_failed = false;

	if(!write_failed) {
		n = write( TransferPipe[1],
				   (char *)&total_bytes,
				   sizeof(filesize_t) );
		if(n != sizeof(filesize_t)) write_failed = true;
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   (char *)&Info.try_again,
				   sizeof(bool) );
		if(n != sizeof(bool)) write_failed = true;
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   (char *)&Info.hold_code,
				   sizeof(int) );
		if(n != sizeof(int)) write_failed = true;
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   (char *)&Info.hold_subcode,
				   sizeof(int) );
		if(n != sizeof(int)) write_failed = true;
	}
	int error_len = Info.error_desc.Length();
	if(error_len) {
		error_len++;  
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   (char *)&error_len,
				   sizeof(int) );
		if(n != sizeof(int)) write_failed = true;
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   Info.error_desc.Value(),
				   error_len );
		if(n != error_len) write_failed = true;
	}

	int spooled_files_len = Info.spooled_files.Length();
	if(spooled_files_len) {
		spooled_files_len++;  
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   (char *)&spooled_files_len,
				   sizeof(int) );
		if(n != sizeof(int)) write_failed = true;
	}
	if(!write_failed) {
		n = write( TransferPipe[1],
				   Info.spooled_files.Value(),
				   spooled_files_len );
		if(n != spooled_files_len) write_failed = true;
	}

	if(write_failed) {
		dprintf(D_ALWAYS,"Failed to write transfer status to pipe (errno %d): %s\n",errno,strerror(errno));
		return false;
	}

	return true;
}
