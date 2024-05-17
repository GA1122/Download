FileTransfer::changeServer(const char* transkey, const char* transsock)
{

	if ( transkey ) {
		if (TransKey) {
			free(TransKey);
		}
		TransKey = strdup(transkey);
	}

	if ( transsock ) {
		if (TransSock) {
			free(TransSock);
		}
		TransSock = strdup(transsock);
	}

	return true;
}
