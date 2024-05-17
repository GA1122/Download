FileTransfer::CommitFiles()
{
	MyString buf;
	MyString newbuf;
	MyString swapbuf;
	const char *file;

	if ( IsClient() ) {
		return;
	}

	int cluster = -1;
	int proc = -1;
	jobAd.LookupInteger(ATTR_CLUSTER_ID, cluster);
	jobAd.LookupInteger(ATTR_PROC_ID, proc);

	priv_state saved_priv = PRIV_UNKNOWN;
	if( want_priv_change ) {
		saved_priv = set_priv( desired_priv_state );
	}

	Directory tmpspool( TmpSpoolSpace, desired_priv_state );

	buf.sprintf("%s%c%s",TmpSpoolSpace,DIR_DELIM_CHAR,COMMIT_FILENAME);
	if ( access(buf.Value(),F_OK) >= 0 ) {

		MyString SwapSpoolSpace;
		SwapSpoolSpace.sprintf("%s.swap",SpoolSpace);
		bool swap_dir_ready = SpooledJobFiles::createJobSwapSpoolDirectory(&jobAd,desired_priv_state);
		if( !swap_dir_ready ) {
			EXCEPT("Failed to create %s",SwapSpoolSpace.Value());
		}

		while ( (file=tmpspool.Next()) ) {
			if ( file_strcmp(file,COMMIT_FILENAME) == MATCH )
				continue;
			buf.sprintf("%s%c%s",TmpSpoolSpace,DIR_DELIM_CHAR,file);
			newbuf.sprintf("%s%c%s",SpoolSpace,DIR_DELIM_CHAR,file);
			swapbuf.sprintf("%s%c%s",SwapSpoolSpace.Value(),DIR_DELIM_CHAR,file);

			if( access(newbuf.Value(),F_OK) >= 0 ) {
				if ( rename(newbuf.Value(),swapbuf.Value()) < 0 ) {
					EXCEPT("FileTransfer CommitFiles failed to move %s to %s: %s",newbuf.Value(),swapbuf.Value(),strerror(errno));
				}
			}

			if ( rotate_file(buf.Value(),newbuf.Value()) < 0 ) {
				EXCEPT("FileTransfer CommitFiles Failed -- What Now?!?!");
			}
		}

		SpooledJobFiles::removeJobSwapSpoolDirectory(cluster,proc);
	}

	tmpspool.Remove_Entire_Directory();
	if( want_priv_change ) {
		ASSERT( saved_priv != PRIV_UNKNOWN );
		set_priv( saved_priv );
	}
}
