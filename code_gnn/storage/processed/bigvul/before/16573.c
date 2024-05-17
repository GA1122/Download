FileTransfer::ExpandFileTransferList( StringList *input_list, FileTransferList &expanded_list )
{
	bool rc = true;

	if( !input_list ) {
		return true;
	}

	if (X509UserProxy && input_list->contains(X509UserProxy)) {
		if( !ExpandFileTransferList( X509UserProxy, "", Iwd, -1, expanded_list ) ) {
			rc = false;
		}
	}

	input_list->rewind();
	char const *path;
	while ( (path=input_list->next()) != NULL ) {
		if(!X509UserProxy || (X509UserProxy && strcmp(path, X509UserProxy) != 0)) {
			if( !ExpandFileTransferList( path, "", Iwd, -1, expanded_list ) ) {
				rc = false;
			}
		}
	}
	return rc;
}
