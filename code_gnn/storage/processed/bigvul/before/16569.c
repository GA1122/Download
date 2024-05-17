FileTransfer::Download(ReliSock *s, bool blocking)
{
	dprintf(D_FULLDEBUG,"entering FileTransfer::Download\n");
	
	if (ActiveTransferTid >= 0) {
		EXCEPT("FileTransfer::Download called during active transfer!\n");
	}

	Info.duration = 0;
	Info.type = DownloadFilesType;
	Info.success = true;
	Info.in_progress = true;
	TransferStart = time(NULL);

	if (blocking) {

		int status = DoDownload( &Info.bytes, (ReliSock *) s );
		Info.duration = time(NULL)-TransferStart;
		Info.success = ( status >= 0 );
		Info.in_progress = false;
		return Info.success;

	} else {

		ASSERT( daemonCore );

		if (pipe(TransferPipe) < 0) {
			dprintf(D_ALWAYS, "pipe failed with errno %d in "
					"FileTransfer::Upload\n", errno);
			return FALSE;
		}

		download_info *info = (download_info *)malloc(sizeof(download_info));
		info->myobj = this;
		ActiveTransferTid = daemonCore->
			Create_Thread((ThreadStartFunc)&FileTransfer::DownloadThread,
						  (void *)info, s, ReaperId);
		if (ActiveTransferTid == FALSE) {
			dprintf(D_ALWAYS,
					"Failed to create FileTransfer DownloadThread!\n");
			ActiveTransferTid = -1;
			free(info);
			return FALSE;
		}
		TransThreadTable->insert(ActiveTransferTid, this);

	}
	
	return 1;
}
