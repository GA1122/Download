FileTransfer::DownloadFiles(bool blocking)
{
	int ret_value;
	ReliSock sock;
	ReliSock *sock_to_use;

	dprintf(D_FULLDEBUG,"entering FileTransfer::DownloadFiles\n");

	if (ActiveTransferTid >= 0) {
		EXCEPT("FileTransfer::DownloadFiles called during active transfer!\n");
	}

	if ( Iwd == NULL ) {
		EXCEPT("FileTransfer: Init() never called");
	}

	if (!simple_init) {
		if ( IsServer() ) {
			EXCEPT("FileTransfer: DownloadFiles called on server side");
		}

		sock.timeout(clientSockTimeout);

		Daemon d( DT_ANY, TransSock );

		if ( !d.connectSock(&sock,0) ) {
			dprintf( D_ALWAYS, "FileTransfer: Unable to connect to server "
					 "%s\n", TransSock );
			return FALSE;
		}

		d.startCommand(FILETRANS_UPLOAD, &sock, 0, NULL, NULL, false, m_sec_session_id);

		sock.encode();

		if ( !sock.put_secret(TransKey) ||
			!sock.end_of_message() ) {
			return 0;
		}

		sock_to_use = &sock;
	} else {
		ASSERT(simple_sock);
		sock_to_use = simple_sock;
	}

	ret_value = Download(sock_to_use,blocking);

	if ( !simple_init && blocking && ret_value == 1 && upload_changed_files ) {
		time(&last_download_time);
		BuildFileCatalog();
		sleep(1);
	}

	return ret_value;
}
