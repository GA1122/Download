FileTransfer::addOutputFile( const char* filename )
{
	if( ! OutputFiles ) {
		OutputFiles = new StringList;
		ASSERT(OutputFiles != NULL);
	}
	else if( OutputFiles->file_contains(filename) ) {
		return true;
	}
	OutputFiles->append( filename );
	return true;
}
