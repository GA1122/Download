FileTransfer::Init( ClassAd *Ad, bool want_check_perms, priv_state priv,
	bool use_file_catalog) 
{
	char buf[ATTRLIST_MAX_EXPRESSION];
	char *dynamic_buf = NULL;

	ASSERT( daemonCore );	 

	if( did_init ) {
		return 1;
	}

	dprintf(D_FULLDEBUG,"entering FileTransfer::Init\n");

	m_use_file_catalog = use_file_catalog;

	simple_init = false;

	if (!TranskeyTable) {
		if (!(TranskeyTable = new TranskeyHashTable(7, compute_transkey_hash)))
		{
			return 0;
		}
		
	}

	if (ActiveTransferTid >= 0) {
		EXCEPT("FileTransfer::Init called during active transfer!");
	}

	if (!TransThreadTable) {
		if (!(TransThreadTable =
			  new TransThreadHashTable(7, compute_transthread_hash))) {
			return 0;
		}
	}


	if ( !CommandsRegistered  ) {
		CommandsRegistered = TRUE;
		daemonCore->Register_Command(FILETRANS_UPLOAD,"FILETRANS_UPLOAD",
				(CommandHandler)&FileTransfer::HandleCommands,
				"FileTransfer::HandleCommands()",NULL,WRITE);
		daemonCore->Register_Command(FILETRANS_DOWNLOAD,"FILETRANS_DOWNLOAD",
				(CommandHandler)&FileTransfer::HandleCommands,
				"FileTransfer::HandleCommands()",NULL,WRITE);
		ReaperId = daemonCore->Register_Reaper("FileTransfer::Reaper",
							(ReaperHandler)&FileTransfer::Reaper,
							"FileTransfer::Reaper()",NULL);
		if (ReaperId == 1) {
			EXCEPT("FileTransfer::Reaper() can not be the default reaper!\n");
		}

		set_seed( time(NULL) + (unsigned long)this + (unsigned long)Ad );
	}

	if (Ad->LookupString(ATTR_TRANSFER_KEY, buf) != 1) {
		char tempbuf[80];
		sprintf(tempbuf,"%x#%x%x%x",++SequenceNum,(unsigned)time(NULL),
			get_random_int(),get_random_int());
		TransKey = strdup(tempbuf);
		user_supplied_key = FALSE;
		sprintf(tempbuf,"%s=\"%s\"",ATTR_TRANSFER_KEY,TransKey);
		Ad->InsertOrUpdate(tempbuf);

		char const *mysocket = global_dc_sinful();
		ASSERT(mysocket);
		Ad->Assign(ATTR_TRANSFER_SOCKET,mysocket);
	} else {
		TransKey = strdup(buf);
		user_supplied_key = TRUE;
	}

	if ( !SimpleInit(Ad, want_check_perms, IsServer(),
			NULL, priv, m_use_file_catalog ) ) 
	{
		return 0;
	}

	if (Ad->LookupString(ATTR_TRANSFER_SOCKET, buf) != 1) {
		return 0;		
	}
	TransSock = strdup(buf);


	buf[0] = '\0';
	if ( IsServer() && upload_changed_files ) {
		CommitFiles();
		MyString filelist;
		const char* current_file = NULL;
		bool print_comma = false;
		Directory spool_space( SpoolSpace, desired_priv_state );
		while ( (current_file=spool_space.Next()) ) {
			if ( UserLogFile && 
				 !file_strcmp(UserLogFile,current_file) ) 
			{
				continue;
			}				

			time_t mod_time;
			filesize_t filesize;
			if ( LookupInFileCatalog(current_file, &mod_time, &filesize) ) {

				if((filesize==-1)) {
					if(spool_space.GetModifyTime() <= mod_time) {
						dprintf( D_FULLDEBUG,
					 		"Not including file %s, t: %ld<=%ld, s: N/A\n",
					 		current_file, spool_space.GetModifyTime(), mod_time);
						continue;
					}
				}
				else if((spool_space.GetModifyTime()==mod_time) &&
						(spool_space.GetFileSize()==filesize) ) {
					dprintf( D_FULLDEBUG, 
						 "Not including file %s, t: %ld, "
						 "s: " FILESIZE_T_FORMAT "\n",
						 current_file, spool_space.GetModifyTime(), spool_space.GetFileSize());
					continue;
				}
				dprintf( D_FULLDEBUG, 
					 "Including changed file %s, t: %ld, %ld, "
					 "s: " FILESIZE_T_FORMAT ", " FILESIZE_T_FORMAT "\n",
					 current_file,
					 spool_space.GetModifyTime(), mod_time,
					 spool_space.GetFileSize(), filesize );
			}

			if ( print_comma ) {
				filelist += ",";
			} else {
				print_comma = true;
			}
			filelist += current_file;			
		}
		if ( print_comma ) {
			MyString intermediateFilesBuf;
			intermediateFilesBuf.sprintf( "%s=\"%s\"",
				ATTR_TRANSFER_INTERMEDIATE_FILES,filelist.Value());
			Ad->InsertOrUpdate(intermediateFilesBuf.Value());
			dprintf(D_FULLDEBUG,"%s\n",buf);
		}
	}
	if ( IsClient() && upload_changed_files ) {
		dynamic_buf = NULL;
		Ad->LookupString(ATTR_TRANSFER_INTERMEDIATE_FILES,&dynamic_buf);
		dprintf(D_FULLDEBUG,"%s=\"%s\"\n",
				ATTR_TRANSFER_INTERMEDIATE_FILES,
				dynamic_buf ? dynamic_buf : "(none)");
		if ( dynamic_buf ) {
			SpooledIntermediateFiles = strnewp(dynamic_buf);
			free(dynamic_buf);
			dynamic_buf = NULL;
		}
	}
	

	if ( IsServer() ) {
		MyString key(TransKey);
		FileTransfer *transobject;
		if ( TranskeyTable->lookup(key,transobject) < 0 ) {
			if ( TranskeyTable->insert(key,this) < 0 ) {
				dprintf(D_ALWAYS,
					"FileTransfer::Init failed to insert key in our table\n");
				return 0;
			}
		} else {
			EXCEPT("FileTransfer: Duplicate TransferKeys!");
		}
	}

	did_init = true;
	
	return 1;
}
