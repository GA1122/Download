FileTransfer::UploadFiles(bool blocking, bool final_transfer)
{
    ReliSock sock;
	ReliSock *sock_to_use;

	StringList changed_files(NULL,",");

	dprintf(D_FULLDEBUG,
		"entering FileTransfer::UploadFiles (final_transfer=%d)\n",
		final_transfer ? 1 : 0);

	if (ActiveTransferTid >= 0) {
		EXCEPT("FileTransfer::UpLoadFiles called during active transfer!\n");
	}

	if ( Iwd == NULL ) {
		EXCEPT("FileTransfer: Init() never called");
	}

	if ( !simple_init && IsServer() ) {
		EXCEPT("FileTransfer: UploadFiles called on server side");
	}

	if ( UserLogFile && TransferUserLog && simple_init && !nullFile( UserLogFile ) ) {
		if ( !InputFiles->file_contains( UserLogFile ) )
			InputFiles->append( UserLogFile );
	}

	m_final_transfer_flag = final_transfer ? 1 : 0;

	ComputeFilesToSend();

	if ( FilesToSend == NULL ) {
		if ( simple_init ) {
			if ( IsClient() ) {
				FilesToSend = InputFiles;
				EncryptFiles = EncryptInputFiles;
				DontEncryptFiles = DontEncryptInputFiles;
			} else {
				FilesToSend = OutputFiles;
				EncryptFiles = EncryptOutputFiles;
				DontEncryptFiles = DontEncryptOutputFiles;
			}
		} else {
			FilesToSend = OutputFiles;
			EncryptFiles = EncryptOutputFiles;
			DontEncryptFiles = DontEncryptOutputFiles;
		}

	}

	if ( !simple_init ) {
		if ( FilesToSend == NULL ) {
			return 1;
		}

		sock.timeout(clientSockTimeout);

		Daemon d( DT_ANY, TransSock );

		if ( !d.connectSock(&sock,0) ) {
			dprintf( D_ALWAYS, "FileTransfer: Unable to connect to server "
					 "%s\n", TransSock );
			return FALSE;
		}

		d.startCommand(FILETRANS_DOWNLOAD, &sock, clientSockTimeout, NULL, NULL, false, m_sec_session_id);

		sock.encode();

		if ( !sock.put_secret(TransKey) ||
			!sock.end_of_message() ) {
			return 0;
		}

		dprintf( D_FULLDEBUG,
				 "FileTransfer::UploadFiles: sent TransKey=%s\n", TransKey );

		sock_to_use = &sock;
	} else {
		ASSERT(simple_sock);
		sock_to_use = simple_sock;
	}


	int retval = Upload(sock_to_use,blocking);

	return( retval );
}
