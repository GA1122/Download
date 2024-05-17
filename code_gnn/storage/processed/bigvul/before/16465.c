void check_tmp_dir(){
#if !defined(WIN32)
	if (!RmFlag) return;

	const char *tmpDir = NULL;
	bool newLock = param_boolean("CREATE_LOCKS_ON_LOCAL_DISK", true);
	if (newLock) {
		FileLock *lock = new FileLock(-1, NULL, NULL);
		tmpDir = lock->GetTempPath();	
		delete lock;
		rec_lock_cleanup(tmpDir, 3);
		if (tmpDir != NULL)
			delete []tmpDir;
	}
  
#endif	
}
