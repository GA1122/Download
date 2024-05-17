FileTransfer::~FileTransfer()
{
	if (daemonCore && ActiveTransferTid >= 0) {
		dprintf(D_ALWAYS, "FileTransfer object destructor called during "
				"active transfer.  Cancelling transfer.\n");
		daemonCore->Kill_Thread(ActiveTransferTid);
		TransThreadTable->remove(ActiveTransferTid);
		ActiveTransferTid = -1;
	}
	if (TransferPipe[0] >= 0) close(TransferPipe[0]);
	if (TransferPipe[1] >= 0) close(TransferPipe[1]);
	if (Iwd) free(Iwd);
	if (ExecFile) free(ExecFile);
	if (UserLogFile) free(UserLogFile);
	if (X509UserProxy) free(X509UserProxy);
	if (SpoolSpace) free(SpoolSpace);
	if (TmpSpoolSpace) free(TmpSpoolSpace);
	if (ExceptionFiles) delete ExceptionFiles;
	if (InputFiles) delete InputFiles;
	if (OutputFiles) delete OutputFiles;
	if (EncryptInputFiles) delete EncryptInputFiles;
	if (EncryptOutputFiles) delete EncryptOutputFiles;
	if (DontEncryptInputFiles) delete DontEncryptInputFiles;
	if (DontEncryptOutputFiles) delete DontEncryptOutputFiles;
	if (OutputDestination) delete OutputDestination;
	if (IntermediateFiles) delete IntermediateFiles;
	if (SpooledIntermediateFiles) delete SpooledIntermediateFiles;
	if (last_download_catalog) {
		CatalogEntry *entry_pointer;
		last_download_catalog->startIterations();
		while(last_download_catalog->iterate(entry_pointer)) {
			delete entry_pointer;
		}
		delete last_download_catalog;
	}
	if (TransSock) free(TransSock);
	if (TransKey) {
		if ( TranskeyTable ) {
			MyString key(TransKey);
			TranskeyTable->remove(key);
			if ( TranskeyTable->getNumElements() == 0 ) {
				delete TranskeyTable;
				TranskeyTable = NULL;
				delete TransThreadTable;
				TransThreadTable = NULL;
			}
		}		
		free(TransKey);
	}	
#ifdef WIN32
	if (perm_obj) delete perm_obj;
#endif
	free(m_sec_session_id);
}
