static void windows_exit(void)
{
	int i;
	HANDLE semaphore;
	char sem_name[11+1+8];  

	sprintf(sem_name, "libusb_init%08X", (unsigned int)GetCurrentProcessId()&0xFFFFFFFF);
	semaphore = CreateSemaphoreA(NULL, 1, 1, sem_name);
	if (semaphore == NULL) {
		return;
	}

	if (WaitForSingleObject(semaphore, INFINITE) != WAIT_OBJECT_0) {
		CloseHandle(semaphore);
		return;
	}

	if (--concurrent_usage < 0) {	 
		for (i=0; i<USB_API_MAX; i++) {
			usb_api_backend[i].exit(SUB_API_NOTSET);
		}
		exit_polling();

		if (timer_thread) {
			SetEvent(timer_request[1]);  
			if (WAIT_OBJECT_0 != WaitForSingleObject(timer_thread, INFINITE)) {
				usbi_dbg("could not wait for timer thread to quit");
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

	ReleaseSemaphore(semaphore, 1, NULL);	 
	CloseHandle(semaphore);
}
