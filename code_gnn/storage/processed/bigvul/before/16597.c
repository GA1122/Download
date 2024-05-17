FileTransfer::Upload(ReliSock *s, bool blocking)
{
	dprintf(D_FULLDEBUG,"entering FileTransfer::Upload\n");

	if (ActiveTransferTid >= 0) {
		EXCEPT("FileTransfer::Upload called during active transfer!\n");
	}

	Info.duration = 0;
	Info.type = UploadFilesType;
	Info.success = true;
	Info.in_progress = true;
	TransferStart = time(NULL);

	if (blocking) {
		int status = DoUpload( &Info.bytes, (ReliSock *)s);
		Info.duration = time(NULL)-TransferStart;
		Info.success = (Info.bytes >= 0) && (status == 0);
		Info.in_progress = false;
		return Info.success;

	} else {

		ASSERT( daemonCore );

		if (pipe(TransferPipe) < 0) {
			dprintf(D_ALWAYS, "pipe failed with errno %d in "
					"FileTransfer::Upload\n", errno);
			return FALSE;
		}

		upload_info *info = (upload_info *)malloc(sizeof(upload_info));
		info->myobj = this;
		ActiveTransferTid = daemonCore->
			Create_Thread((ThreadStartFunc)&FileTransfer::UploadThread,
						  (void *)info, s, ReaperId);
		if (ActiveTransferTid == FALSE) {
			dprintf(D_ALWAYS, "Failed to create FileTransfer UploadThread!\n");
			free(info);
			ActiveTransferTid = -1;
			return FALSE;
		}
		TransThreadTable->insert(ActiveTransferTid, this);
	}
		
	return 1;
}
