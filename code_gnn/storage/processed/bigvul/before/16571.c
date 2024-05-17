FileTransfer::DownloadThread(void *arg, Stream *s)
{
	filesize_t	total_bytes;

	dprintf(D_FULLDEBUG,"entering FileTransfer::DownloadThread\n");
	FileTransfer * myobj = ((download_info *)arg)->myobj;
	int status = myobj->DoDownload( &total_bytes, (ReliSock *)s );

	if(!myobj->WriteStatusToTransferPipe(total_bytes)) {
		return 0;
	}
	return ( status == 0 );
}
