FileTransfer::ExpandFileTransferList( char const *src_path, char const *dest_dir, char const *iwd, int max_depth, FileTransferList &expanded_list )
{
	ASSERT( src_path );
	ASSERT( dest_dir );
	ASSERT( iwd );

	expanded_list.push_back( FileTransferItem() );
	FileTransferItem &file_xfer_item = expanded_list.back();

	file_xfer_item.src_name = src_path;
	file_xfer_item.dest_dir = dest_dir;

	if( IsUrl(src_path) ) {
		return true;
	}

	std::string full_src_path;
	if( is_relative_to_cwd( src_path ) ) {
		full_src_path = iwd;
		if( full_src_path.length() > 0 ) {
			full_src_path += DIR_DELIM_CHAR;
		}
	}
	full_src_path += src_path;

	StatInfo st( full_src_path.c_str() );

	if( st.Error() != 0 ) {
		return false;
	}

#ifndef WIN32
	file_xfer_item.file_mode = (condor_mode_t)st.GetMode();
#endif

	size_t srclen = file_xfer_item.src_name.length();
	bool trailing_slash = srclen > 0 && src_path[srclen-1] == DIR_DELIM_CHAR;

	file_xfer_item.is_symlink = st.IsSymlink();
	file_xfer_item.is_directory = st.IsDirectory();

	if( !file_xfer_item.is_directory ) {
		return true;
	}

	if( !trailing_slash && file_xfer_item.is_symlink ) {
		return true;
	}

	if( max_depth == 0 ) {
		return true;  
	}
	if( max_depth > 0 ) {
		max_depth--;
	}

	std::string dest_dir_buf;
	if( trailing_slash ) {
		expanded_list.pop_back();
	}
	else {
		dest_dir_buf = dest_dir;
		if( dest_dir_buf.length() > 0 ) {
			dest_dir_buf += DIR_DELIM_CHAR;
		}
		dest_dir_buf += condor_basename(src_path);
		dest_dir = dest_dir_buf.c_str();
	}

	Directory dir( &st );
	dir.Rewind();

	bool rc = true;
	char const *file_in_dir;
	while( (file_in_dir=dir.Next()) != NULL ) {

		std::string file_full_path = src_path;
		if( !trailing_slash ) {
			file_full_path += DIR_DELIM_CHAR;
		}
		file_full_path += file_in_dir;

		if( !ExpandFileTransferList( file_full_path.c_str(), dest_dir, iwd, max_depth, expanded_list ) ) {
			rc = false;
		}
	}

	return rc;
}
