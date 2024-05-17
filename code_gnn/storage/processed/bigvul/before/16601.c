FileTransfer::addFileToExeptionList( const char* filename )
{
	if ( !ExceptionFiles ) {
		ExceptionFiles = new StringList;
		ASSERT ( NULL != ExceptionFiles );
	} else if ( ExceptionFiles->file_contains ( filename ) ) {
		return true;
	}
	ExceptionFiles->append ( filename );
	return true;
}
