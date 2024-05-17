FileTransfer::ComputeFilesToSend()
{
	StringList final_files_to_send(NULL,",");
	if (IntermediateFiles) delete(IntermediateFiles);
	IntermediateFiles = NULL;
	FilesToSend = NULL;
	EncryptFiles = NULL;
	DontEncryptFiles = NULL;

	if ( upload_changed_files && last_download_time > 0 ) {

		if ( m_final_transfer_flag && SpooledIntermediateFiles ) {
			final_files_to_send.initializeFromString(SpooledIntermediateFiles);
		}

		Directory dir( Iwd, desired_priv_state );

		const char *proxy_file = NULL;
		MyString proxy_file_buf;		
		if(jobAd.LookupString(ATTR_X509_USER_PROXY, proxy_file_buf)) {			
			proxy_file = condor_basename(proxy_file_buf.Value());
		}

		const char *f;
		while( (f=dir.Next()) ) {
			if ( MATCH == file_strcmp ( f, "condor_exec." ) ) {
				dprintf ( D_FULLDEBUG, "Skipping %s\n", f );
				continue;
			}			
			if( proxy_file && file_strcmp(f, proxy_file) == MATCH ) {
				dprintf( D_FULLDEBUG, "Skipping %s\n", f );
				continue;
			}

			if ( dir.IsDirectory() ) {
				dprintf( D_FULLDEBUG, "Skipping dir %s\n", f );
				continue;
			}

			bool send_it = false;


			filesize_t filesize;
			time_t modification_time;
			if ( ExceptionFiles && ExceptionFiles->file_contains(f) ) {
				dprintf ( 
					D_FULLDEBUG, 
					"Skipping file in exception list: %s\n", 
					f );
				continue;
			} else if ( !LookupInFileCatalog(f, &modification_time, &filesize) ) {
				dprintf( D_FULLDEBUG, 
						 "Sending new file %s, time==%ld, size==%ld\n",	
						 f, dir.GetModifyTime(), (long) dir.GetFileSize() );
				send_it = true;
			}
			else if (final_files_to_send.file_contains(f)) {
				dprintf( D_FULLDEBUG, 
						 "Sending previously changed file %s\n", f);
				send_it = true;
			}
			else if (OutputFiles && OutputFiles->file_contains(f)) {
				dprintf(D_FULLDEBUG, 
				        "Sending dynamically added output file %s\n",
				        f);
				send_it = true;
			}
			else if (filesize == -1) {
				if (dir.GetModifyTime() > modification_time) {
					dprintf( D_FULLDEBUG, 
						 "Sending changed file %s, t: %ld, %ld, "
						 "s: " FILESIZE_T_FORMAT ", N/A\n",
						 f, dir.GetModifyTime(), modification_time,
						 dir.GetFileSize());
					send_it = true;
				} else {
					dprintf( D_FULLDEBUG,
					 	"Skipping file %s, t: %ld<=%ld, s: N/A\n",
					 	f, dir.GetModifyTime(), modification_time);
					continue;
				}
			}
			else if ((filesize != dir.GetFileSize()) ||
					(modification_time != dir.GetModifyTime()) ) {
				dprintf( D_FULLDEBUG, 
					 "Sending changed file %s, t: %ld, %ld, "
					 "s: " FILESIZE_T_FORMAT ", " FILESIZE_T_FORMAT "\n",
					 f, dir.GetModifyTime(), modification_time,
					 dir.GetFileSize(), filesize );
				send_it = true;
			}
			else {
				dprintf( D_FULLDEBUG,
						 "Skipping file %s, t: %"PRIi64"==%"PRIi64
						 ", s: %"PRIi64"==%"PRIi64"\n",
						 f,
						 (PRIi64_t)dir.GetModifyTime(),
						 (PRIi64_t)modification_time,
						 (PRIi64_t)dir.GetFileSize(),
						 (PRIi64_t)filesize );
				continue;
			}
			if(send_it) {
				if (!IntermediateFiles) {
					IntermediateFiles = new StringList(NULL,",");
					FilesToSend = IntermediateFiles;
					EncryptFiles = EncryptOutputFiles;
					DontEncryptFiles = DontEncryptOutputFiles;
				}
				if ( IntermediateFiles->file_contains(f) == FALSE ) {
					IntermediateFiles->append(f);
				}
			}
		}
	}
}
