FileTransfer::DoDownload( filesize_t *total_bytes, ReliSock *s)
{
	int rc;
	int reply = 0;
	filesize_t bytes=0;
	MyString filename;;
	MyString fullname;
	char *tmp_buf = NULL;
	int final_transfer = 0;
	bool download_success = true;
	bool try_again = true;
	int hold_code = 0;
	int hold_subcode = 0;
	MyString error_buf;
	int delegation_method = 0;  
	time_t start, elapsed;
	bool I_go_ahead_always = false;
	bool peer_goes_ahead_always = false;
	DCTransferQueue xfer_queue(m_xfer_queue_contact_info);
	CondorError errstack;

	priv_state saved_priv = PRIV_UNKNOWN;
	*total_bytes = 0;

	bool want_fsync = ( ((IsClient() && !simple_init) ||   
						 (IsServer() && simple_init))      
						 && upload_changed_files );

	dprintf(D_FULLDEBUG,"entering FileTransfer::DoDownload sync=%d\n",
					want_fsync ? 1 : 0);

	s->decode();

	bool socket_default_crypto = s->get_encryption();

	if( !s->code(final_transfer) ) {
		dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
		return_and_resetpriv( -1 );
	}
	if( !s->end_of_message() ) {
		dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
		return_and_resetpriv( -1 );
	}	

	if( !final_transfer && IsServer() ) {
		SpooledJobFiles::createJobSpoolDirectory(&jobAd,desired_priv_state);
	}

	for (;;) {
		if( !s->code(reply) ) {
			dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
			return_and_resetpriv( -1 );
		}
		if( !s->end_of_message() ) {
			dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
			return_and_resetpriv( -1 );
		}
		dprintf( D_SECURITY, "FILETRANSFER: incoming file_command is %i\n", reply);
		if( !reply ) {
			break;
		}
		if (reply == 2) {
			s->set_crypto_mode(true);
		} else if (reply == 3) {
			s->set_crypto_mode(false);
		}
		else {
			s->set_crypto_mode(socket_default_crypto);
		}

		tmp_buf = NULL;
		if( !s->code(tmp_buf) ) {
			dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
			return_and_resetpriv( -1 );
		}
		filename = tmp_buf;
		free( tmp_buf );
		tmp_buf = NULL;


			 
		if( want_priv_change && saved_priv == PRIV_UNKNOWN ) {
			saved_priv = set_priv( desired_priv_state );
		}

		if( !LegalPathInSandbox(filename.Value(),Iwd) ) {

			download_success = false;
			try_again = false;
			hold_code = CONDOR_HOLD_CODE_DownloadFileError;
			hold_subcode = EPERM;

			error_buf.sprintf_cat(
				" Attempt to write to illegal sandbox path: %s",
				filename.Value());

			dprintf(D_ALWAYS,"DoDownload: attempt to write to illegal sandbox path by our peer %s: %s.\n",
					s->peer_description(),
					filename.Value());

			filename = NULL_FILE;
		}

		if( !strcmp(filename.Value(),NULL_FILE) ) {
			fullname = filename;
		}
		else if( final_transfer || IsClient() ) {
			MyString remap_filename;
			if(filename_remap_find(download_filename_remaps.Value(),filename.Value(),remap_filename)) {
				if(!is_relative_to_cwd(remap_filename.Value())) {
					fullname = remap_filename;
				}
				else {
					fullname.sprintf("%s%c%s",Iwd,DIR_DELIM_CHAR,remap_filename.Value());
				}
				dprintf(D_FULLDEBUG,"Remapped downloaded file from %s to %s\n",filename.Value(),remap_filename.Value());
			}
			else {
				fullname.sprintf("%s%c%s",Iwd,DIR_DELIM_CHAR,filename.Value());
			}
#ifdef WIN32
			if ( perm_obj && (perm_obj->write_access(fullname.Value()) != 1) ) {
				error_buf.sprintf("Permission denied to write file %s!",
				                   fullname.Value());
				dprintf(D_ALWAYS,"DoDownload: %s\n",error_buf.Value());
				download_success = false;
				try_again = false;
				hold_code = CONDOR_HOLD_CODE_DownloadFileError;
				hold_subcode = EPERM;

				fullname = NULL_FILE;
			}
#endif
		} else {
			fullname.sprintf("%s%c%s",TmpSpoolSpace,DIR_DELIM_CHAR,filename.Value());
		}

		if( PeerDoesGoAhead ) {
			if( !s->end_of_message() ) {
				dprintf(D_FULLDEBUG,"DoDownload: failed on eom before GoAhead: exiting at %d\n",__LINE__);
				return_and_resetpriv( -1 );
			}

			if( !I_go_ahead_always ) {
				if( !ObtainAndSendTransferGoAhead(xfer_queue,true,s,fullname.Value(),I_go_ahead_always) ) {
					dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
					return_and_resetpriv( -1 );
				}
			}

			if( !peer_goes_ahead_always ) {

				if( !ReceiveTransferGoAhead(s,fullname.Value(),true,peer_goes_ahead_always) ) {
					dprintf(D_FULLDEBUG, "DoDownload: exiting at %d\n",__LINE__);
					return_and_resetpriv( -1 );
				}
			}

			s->decode();
		}

		start = time(NULL);


		if (reply == 999) {
			ClassAd file_info;
			if (!file_info.initFromStream(*s)) {
				dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
				return_and_resetpriv( -1 );
			}

				
			int      subcommand = 0;
			if(!file_info.LookupInteger("Result",subcommand)) {
				subcommand = -1;
			}


			if(subcommand == 7) {
				
				MyString rt_src;
				MyString rt_dst;
				MyString rt_err;
				int      rt_result = 0;
				if(!file_info.LookupInteger("Result",rt_result)) {
					rt_result = -1;
				}

				if(!file_info.LookupString("Filename", rt_src)) {
					rt_src = "<null>";
				}

				if(!file_info.LookupString("OutputDestination", rt_dst)) {
					rt_dst = "<null>";
				}

				if(!file_info.LookupString("ErrorString", rt_err)) {
					rt_err = "<null>";
				}

				dprintf(D_ALWAYS, "DoDownload: other side transferred %s to %s and got result %i\n",
						rt_src.Value(), rt_dst.Value(), rt_result );

				if(rt_result == 0) {
					rc = 0;
				} else {
					rc = 0; 

					error_buf.sprintf(
						"%s at %s failed due to remote transfer hook error: %s",
						get_mySubSystem()->getName(),
						s->my_ip_str(),fullname.Value());
					download_success = false;
					try_again = false;
					hold_code = CONDOR_HOLD_CODE_DownloadFileError;
					hold_subcode = rt_result;

					dprintf(D_ALWAYS,
						"DoDownload: consuming rest of transfer and failing "
						"after encountering the following error: %s\n",
						error_buf.Value());
				}
			} else {
				dprintf(D_ALWAYS, "FILETRANSFER: unrecognized subcommand %i! skipping!\n", subcommand);
				file_info.dPrint(D_FULLDEBUG);
				
				rc = 0;
			}
		} else if (reply == 5) {

			MyString URL;

			if (!s->code(URL)) {
				dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
				return_and_resetpriv( -1 );
			}

			dprintf( D_FULLDEBUG, "DoDownload: doing a URL transfer: (%s) to (%s)\n", URL.Value(), fullname.Value());

			rc = InvokeFileTransferPlugin(errstack, URL.Value(), fullname.Value(), LocalProxyName.Value());


		} else if ( reply == 4 ) {
			if ( PeerDoesGoAhead || s->end_of_message() ) {
				rc = s->get_x509_delegation( &bytes, fullname.Value() );
				dprintf( D_FULLDEBUG,
				         "DoDownload: get_x509_delegation() returned %d\n",
				         rc );
				if (rc == 0) {
					LocalProxyName = fullname;
				}
			} else {
				rc = -1;
			}
			delegation_method = 1; 
		} else if( reply == 6 ) {  
			condor_mode_t file_mode = NULL_FILE_PERMISSIONS;
			if( !s->code(file_mode) ) {
				rc = -1;
				dprintf(D_ALWAYS,"DoDownload: failed to read mkdir mode.\n");
			}
			else {
				rc = mkdir(fullname.Value(),file_mode);
				if( rc == -1 && errno == EEXIST ) {
					StatInfo st( fullname.Value() );
					if( !st.Error() && st.IsDirectory() ) {
						dprintf(D_FULLDEBUG,"Requested to create directory but using existing one: %s\n",fullname.Value());
						rc = 0;
					}
					else if( !strcmp(fullname.Value(),NULL_FILE) ) {
						rc = 0;
					}
					else {
						remove(fullname.Value());
						rc = mkdir(fullname.Value(),file_mode);
					}
				}
				if( rc == -1 ) {
					rc = 0; 

					int the_error = errno;
					error_buf.sprintf(
						"%s at %s failed to create directory %s: %s (errno %d)",
						get_mySubSystem()->getName(),
						s->my_ip_str(),fullname.Value(),
						strerror(the_error),the_error);
					download_success = false;
					try_again = false;
					hold_code = CONDOR_HOLD_CODE_DownloadFileError;
					hold_subcode = the_error;

					dprintf(D_ALWAYS,
						"DoDownload: consuming rest of transfer and failing "
						"after encountering the following error: %s\n",
						error_buf.Value());
				}
			}
		} else if ( TransferFilePermissions ) {
			rc = s->get_file_with_permissions( &bytes, fullname.Value() );
		} else {
			rc = s->get_file( &bytes, fullname.Value() );
		}

		elapsed = time(NULL)-start;

		if( rc < 0 ) {
			int the_error = errno;
			error_buf.sprintf("%s at %s failed to receive file %s",
			                  get_mySubSystem()->getName(),
							  s->my_ip_str(),fullname.Value());
			download_success = false;
			if(rc == GET_FILE_OPEN_FAILED || rc == GET_FILE_WRITE_FAILED ||
					rc == GET_FILE_PLUGIN_FAILED) {

				if (rc == GET_FILE_PLUGIN_FAILED) {
					error_buf.sprintf_cat(": %s", errstack.getFullText());
				} else {
					error_buf.replaceString("receive","write to");
					error_buf.sprintf_cat(": (errno %d) %s",the_error,strerror(the_error));
				}


				try_again = false;
				hold_code = CONDOR_HOLD_CODE_DownloadFileError;
				hold_subcode = the_error;

				dprintf(D_ALWAYS,
						"DoDownload: consuming rest of transfer and failing "
						"after encountering the following error: %s\n",
						error_buf.Value());
			}
			else {
				try_again = true;

				dprintf(D_ALWAYS,"DoDownload: %s\n",error_buf.Value());

				SendTransferAck(s,download_success,try_again,hold_code,hold_subcode,error_buf.Value());

				dprintf(D_FULLDEBUG,"DoDownload: exiting at %d\n",__LINE__);
				return_and_resetpriv( -1 );
			}
		}

		if ( want_fsync ) {
			struct utimbuf timewrap;

			time_t current_time = time(NULL);
			timewrap.actime = current_time;		 
			timewrap.modtime = current_time - 180;	 

			utime(fullname.Value(),&timewrap);
		}

		if( !s->end_of_message() ) {
			return_and_resetpriv( -1 );
		}
		*total_bytes += bytes;

#ifdef HAVE_EXT_POSTGRESQL
	        file_transfer_record record;
		record.fullname = fullname.Value();
		record.bytes = bytes;
		record.elapsed  = elapsed;
    
		char daemon[16]; daemon[15] = '\0';
		strncpy(daemon, get_mySubSystem()->getName(), 15);
		record.daemon = daemon;

		record.sockp =s;
		record.transfer_time = start;
		record.delegation_method_id = delegation_method;
		file_transfer_db(&record, &jobAd);
#else
		if (delegation_method) {}
		if (elapsed) {}
#endif
	}

	s->set_crypto_mode(socket_default_crypto);

#ifdef WIN32
	bytesRcvd += (float)(signed __int64)(*total_bytes);
#else
	bytesRcvd += (*total_bytes);
#endif

	bool upload_success = false;
	MyString upload_error_buf;
	bool upload_try_again = true;
	int upload_hold_code = 0;
	int upload_hold_subcode = 0;
	GetTransferAck(s,upload_success,upload_try_again,upload_hold_code,
				   upload_hold_subcode,upload_error_buf);
	if(!upload_success) {

		char const *peer_ip_str = "disconnected socket";
		if(s->type() == Stream::reli_sock) {
			peer_ip_str = ((Sock *)s)->get_sinful_peer();
		}

		MyString download_error_buf;
		download_error_buf.sprintf("%s failed to receive file(s) from %s",
						  get_mySubSystem()->getName(),peer_ip_str);
		error_buf.sprintf("%s; %s",
						  upload_error_buf.Value(),
						  download_error_buf.Value());
		dprintf(D_ALWAYS,"DoDownload: %s\n",error_buf.Value());

		download_success = false;
		SendTransferAck(s,download_success,upload_try_again,upload_hold_code,
						upload_hold_subcode,download_error_buf.Value());

		Info.error_desc = error_buf.Value();

		dprintf( D_FULLDEBUG, "DoDownload: exiting with upload errors\n" );
		return_and_resetpriv( -1 );
	}

	if( !download_success ) {
		SendTransferAck(s,download_success,try_again,hold_code,
						hold_subcode,error_buf.Value());

		dprintf( D_FULLDEBUG, "DoDownload: exiting with download errors\n" );
		return_and_resetpriv( -1 );
	}

	if ( !final_transfer && IsServer() ) {
		MyString buf;
		int fd;

		buf.sprintf("%s%c%s",TmpSpoolSpace,DIR_DELIM_CHAR,COMMIT_FILENAME);
#if defined(WIN32)
		if ((fd = safe_open_wrapper_follow(buf.Value(), O_WRONLY | O_CREAT | O_TRUNC | 
			_O_BINARY | _O_SEQUENTIAL, 0644)) < 0)
#else
		if ((fd = safe_open_wrapper_follow(buf.Value(), O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
#endif
		{
			dprintf(D_ALWAYS, 
				"FileTransfer::DoDownload failed to write commit file\n");
			return_and_resetpriv( -1 );
		}

		::close(fd);

		CommitFiles();

	}

	download_success = true;
	SendTransferAck(s,download_success,try_again,hold_code,hold_subcode,NULL);

	return_and_resetpriv( 0 );
}
