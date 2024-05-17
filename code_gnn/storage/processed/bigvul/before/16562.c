bool FileTransfer::BuildFileCatalog(time_t spool_time, const char* iwd, FileCatalogHashTable **catalog) {

	if (!iwd) {
		iwd = Iwd;
	}

	if (!catalog) {
		catalog = &last_download_catalog;
	}

	if (*catalog) {
		CatalogEntry *entry_pointer;

		(*catalog)->startIterations();
		while((*catalog)->iterate(entry_pointer)) {
			delete entry_pointer;
		}
		delete (*catalog);
	}

	(*catalog) = new FileCatalogHashTable(997, compute_filename_hash);

	 
	if (m_use_file_catalog == false) {
		 
		return true;
	}

	Directory file_iterator(iwd);
	const char * f = NULL;
	while( (f = file_iterator.Next()) ) {
		if (!file_iterator.IsDirectory()) {
			CatalogEntry *tmpentry = 0;
			tmpentry = new CatalogEntry;
			if (spool_time) {
				tmpentry->modification_time = spool_time;
				tmpentry->filesize = -1;
			} else {
				tmpentry->modification_time = file_iterator.GetModifyTime();
				tmpentry->filesize = file_iterator.GetFileSize();
			}
			MyString fn = f;
			(*catalog)->insert(fn, tmpentry);
		}
	}

	return true;
}
