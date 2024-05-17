FileTransfer::UploadThread(void *arg, Stream *s)
{
	dprintf(D_FULLDEBUG,"entering FileTransfer::UploadThread\n");
	FileTransfer * myobj = ((upload_info *)arg)->myobj;
	filesize_t	total_bytes;
	int status = myobj->DoUpload( &total_bytes, (ReliSock *)s );
	if(!myobj->WriteStatusToTransferPipe(total_bytes)) {
		return 0;
	}
	return ( status >= 0 );
}
