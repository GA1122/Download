bool FileTransfer::LookupInFileCatalog(const char *fname, time_t *mod_time, filesize_t *filesize) {
	CatalogEntry *entry = 0;
	MyString fn = fname;
	if (last_download_catalog->lookup(fn, entry) == 0) {

		if (mod_time) {
			*mod_time = entry->modification_time;
		}

		if (filesize) {
			*filesize = entry->filesize;
		}

		return true;
	} else {
		return false;
	}
}
