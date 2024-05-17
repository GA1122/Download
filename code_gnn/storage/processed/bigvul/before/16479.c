void rec_lock_cleanup(const char *path, int depth, bool remove_self) {
#if !defined(WIN32)
	FileLock *lock = NULL;
	if (depth == 0) {
		lock = new FileLock(path, true, true);
		delete lock;
		return ;
	}
	Directory *dir = new Directory(path);
	if (dir == NULL) {
		return;
	}
	const char *entry;
	while ((entry = dir->Next()) != 0) {
		if (!dir->IsDirectory() && depth > 1) {  
			lock = new FileLock(path, false, true);
			bool result = lock->obtain(WRITE_LOCK);
			if (!result) {
					dprintf(D_FULLDEBUG, "Cannot lock %s\n", path);
			}
			int res = unlink(dir->GetFullPath());
			if (res != 0) {
				dprintf(D_FULLDEBUG, "Cannot delete %s (%s) \n", path, strerror(errno));
			}
			delete lock;
		} else {
			rec_lock_cleanup(dir->GetFullPath(), depth-1, true);
		}
	}
	if (remove_self) {		
		int res = rmdir(path);
		if (res != 0) {
			dprintf(D_FULLDEBUG, "Directory %s could not be removed.\n", path);
		}
	}
	
	delete dir;
#endif
}	
