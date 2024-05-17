GF_GlobalLock * gf_global_resource_lock(const char * resourceName) {
#ifdef WIN32
#ifdef _WIN32_WCE
	unsigned short sWResourceName[MAX_PATH];
#endif
	DWORD lastErr;
	GF_GlobalLock *lock = gf_malloc(sizeof(GF_GlobalLock));
	lock->resourceName = gf_strdup(resourceName);

	 
#ifdef _WIN32_WCE
	CE_CharToWide((char *)resourceName, sWResourceName);
	lock->hMutex = CreateMutex(NULL, TRUE, sWResourceName);
#else
	lock->hMutex = CreateMutex(NULL, TRUE, resourceName);
#endif
	lastErr = GetLastError();
	if (lastErr && lastErr == ERROR_ALREADY_EXISTS)
		return NULL;
	if (!lock->hMutex)
	{
		GF_LOG(GF_LOG_ERROR, GF_LOG_MUTEX, ("[Mutex] Couldn't create mutex for global lock: %d\n", lastErr));
		return NULL;
	}

	 
	switch (WaitForSingleObject(lock->hMutex, INFINITE)) {
	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
		assert(0);  
		GF_LOG(GF_LOG_ERROR, GF_LOG_MUTEX, ("[Mutex] Couldn't get the global lock\n"));
		gf_global_resource_unlock(lock);
		return NULL;
	}

	return lock;
#else  
	return gf_create_PID_file(resourceName);
#endif  
}
