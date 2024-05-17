FileTransfer::ExpandInputFileList( char const *input_list, char const *iwd, MyString &expanded_list, MyString &error_msg )
{
	bool result = true;
	StringList input_files(input_list,",");
	input_files.rewind();
	char const *path;
	while( (path=input_files.next()) != NULL ) {
		bool needs_expansion = false;

		size_t pathlen = strlen(path);
		bool trailing_slash = pathlen > 0 && path[pathlen-1] == DIR_DELIM_CHAR;

		if( trailing_slash && !IsUrl(path) ) {
			needs_expansion = true;
		}

		if( !needs_expansion ) {
			expanded_list.append_to_list(path,",");
		}
		else {
			FileTransferList filelist;
			if( !ExpandFileTransferList( path, "", iwd, 1, filelist ) ) {
				error_msg.sprintf_cat("Failed to expand '%s' in transfer input file list. ",path);
				result = false;
			}
			FileTransferList::iterator filelist_it;
			for( filelist_it = filelist.begin();
				 filelist_it != filelist.end();
				 filelist_it++ )
			{
				expanded_list.append_to_list(filelist_it->srcName(),",");
			}
		}
	}
	return result;
}
