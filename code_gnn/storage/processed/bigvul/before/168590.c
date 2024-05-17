static int windows_init(struct libusb_context *ctx)
{
	int i, r = LIBUSB_ERROR_OTHER;
	OSVERSIONINFO os_version;
	HANDLE semaphore;
	char sem_name[11+1+8];  

	sprintf(sem_name, "libusb_init%08X", (unsigned int)GetCurrentProcessId()&0xFFFFFFFF);
	semaphore = CreateSemaphoreA(NULL, 1, 1, sem_name);
	if (semaphore == NULL) {
		usbi_err(ctx, "could not create semaphore: %s", windows_error_str(0));
		return LIBUSB_ERROR_NO_MEM;
	}

	if (WaitForSingleObject(semaphore, INFINITE) != WAIT_OBJECT_0) {
		usbi_err(ctx, "failure to access semaphore: %s", windows_error_str(0));
		CloseHandle(semaphore);
		return LIBUSB_ERROR_NO_MEM;
	}

	if ( ++concurrent_usage == 0 ) {	 
		memset(&os_version, 0, sizeof(OSVERSIONINFO));
		os_version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		windows_version = WINDOWS_UNSUPPORTED;
		if ((GetVersionEx(&os_version) != 0) && (os_version.dwPlatformId == VER_PLATFORM_WIN32_NT)) {
			if ((os_version.dwMajorVersion == 5) && (os_version.dwMinorVersion == 1)) {
				windows_version = WINDOWS_XP;
			} else if ((os_version.dwMajorVersion == 5) && (os_version.dwMinorVersion == 2)) {
				windows_version = WINDOWS_2003;	 
			} else if (os_version.dwMajorVersion >= 6) {
				windows_version = WINDOWS_VISTA_AND_LATER;
			}
		}
		if (windows_version == WINDOWS_UNSUPPORTED) {
			usbi_err(ctx, "This version of Windows is NOT supported");
			r = LIBUSB_ERROR_NOT_SUPPORTED;
			goto init_exit;
		}

		usbi_mutex_init(&autoclaim_lock, NULL);

		init_polling();

		if (init_dlls() != LIBUSB_SUCCESS) {
			usbi_err(ctx, "could not resolve DLL functions");
			return LIBUSB_ERROR_NOT_FOUND;
		}

		for (i=0; i<USB_API_MAX; i++) {
			usb_api_backend[i].init(SUB_API_NOTSET, ctx);
		}

		r = LIBUSB_ERROR_NO_MEM;
		for (i = 0; i < 2; i++) {
			timer_request[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
			if (timer_request[i] == NULL) {
				usbi_err(ctx, "could not create timer request event %d - aborting", i);
				goto init_exit;
			}
		}
		timer_response = CreateSemaphore(NULL, 0, MAX_TIMER_SEMAPHORES, NULL);
		if (timer_response == NULL) {
			usbi_err(ctx, "could not create timer response semaphore - aborting");
			goto init_exit;
		}
		timer_mutex = CreateMutex(NULL, FALSE, NULL);
		if (timer_mutex == NULL) {
			usbi_err(ctx, "could not create timer mutex - aborting");
			goto init_exit;
		}
		timer_thread = (HANDLE)_beginthreadex(NULL, 0, windows_clock_gettime_threaded, NULL, 0, NULL);
		if (timer_thread == NULL) {
			usbi_err(ctx, "Unable to create timer thread - aborting");
			goto init_exit;
		}
		SetThreadAffinityMask(timer_thread, 0);

		if (WaitForSingleObject(timer_response, INFINITE) != WAIT_OBJECT_0) {
			usbi_err(ctx, "Failed to wait for timer thread to become ready - aborting");
			goto init_exit;
		}

		htab_create(ctx, HTAB_SIZE);
	}
	r = LIBUSB_SUCCESS;

init_exit:  
	if (!concurrent_usage && r != LIBUSB_SUCCESS) {  
		if (timer_thread) {
			SetEvent(timer_request[1]);  
			if (WAIT_OBJECT_0 != WaitForSingleObject(timer_thread, INFINITE)) {
				usbi_warn(ctx, "could not wait for timer thread to quit");
				TerminateThread(timer_thread, 1);  
			}
			CloseHandle(timer_thread);
			timer_thread = NULL;
		}
		for (i = 0; i < 2; i++) {
			if (timer_request[i]) {
				CloseHandle(timer_request[i]);
				timer_request[i] = NULL;
			}
		}
		if (timer_response) {
			CloseHandle(timer_response);
			timer_response = NULL;
		}
		if (timer_mutex) {
			CloseHandle(timer_mutex);
			timer_mutex = NULL;
		}
		htab_destroy();
	}

	if (r != LIBUSB_SUCCESS)
		--concurrent_usage;  

	ReleaseSemaphore(semaphore, 1, NULL);	 
	CloseHandle(semaphore);
	return r;
}
