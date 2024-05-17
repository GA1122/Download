Bool gf_sys_get_rti_os(u32 refresh_time_ms, GF_SystemRTInfo *rti, u32 flags)
{
#if defined(_WIN32_WCE)
	THREADENTRY32 tentry;
	u64 total_cpu_time, process_cpu_time;
	DWORD orig_perm;
#endif
	MEMORYSTATUS ms;
	u64 creation, exit, kernel, user, process_k_u_time, proc_idle_time, proc_k_u_time;
	u32 entry_time;
	HANDLE hSnapShot;

	assert(sys_init);

	if (!rti) return GF_FALSE;

	proc_idle_time = proc_k_u_time = process_k_u_time = 0;

	entry_time = gf_sys_clock();
	if (last_update_time && (entry_time - last_update_time < refresh_time_ms)) {
		memcpy(rti, &the_rti, sizeof(GF_SystemRTInfo));
		return GF_FALSE;
	}

	if (flags & GF_RTI_SYSTEM_MEMORY_ONLY) {
		memset(rti, 0, sizeof(GF_SystemRTInfo));
		rti->sampling_instant = last_update_time;
		GlobalMemoryStatus(&ms);
		rti->physical_memory = ms.dwTotalPhys;
		rti->physical_memory_avail = ms.dwAvailPhys;
#ifdef GPAC_MEMORY_TRACKING
		rti->gpac_memory = (u64) gpac_allocated_memory;
#endif
		return GF_TRUE;
	}

#if defined (_WIN32_WCE)

	total_cpu_time = process_cpu_time = 0;

	 
	orig_perm = GetCurrentPermissions();
	SetProcPermissions(0xFFFFFFFF);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapShot) {
		tentry.dwSize = sizeof(THREADENTRY32);
		the_rti.thread_count = 0;
		 
		if (Thread32First(hSnapShot, &tentry)) {
			do {
				 
				if (GetThreadTimes( (HANDLE) tentry.th32ThreadID, (FILETIME *) &creation, (FILETIME *) &exit, (FILETIME *) &kernel, (FILETIME *) &user)) {
					total_cpu_time += user + kernel;
					if (tentry.th32OwnerProcessID==the_rti.pid) {
						process_cpu_time += user + kernel;
						the_rti.thread_count ++;
					}
				}
			} while (Thread32Next(hSnapShot, &tentry));
		}
		CloseToolhelp32Snapshot(hSnapShot);
	}

	if (flags & GF_RTI_PROCESS_MEMORY) {
		HEAPLIST32 hlentry;
		HEAPENTRY32 hentry;
		the_rti.process_memory = 0;
		hlentry.dwSize = sizeof(HEAPLIST32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, the_rti.pid);
		if (hSnapShot && Heap32ListFirst(hSnapShot, &hlentry)) {
			do {
				hentry.dwSize = sizeof(hentry);
				if (Heap32First(hSnapShot, &hentry, hlentry.th32ProcessID, hlentry.th32HeapID)) {
					do {
						the_rti.process_memory += hentry.dwBlockSize;
					} while (Heap32Next(hSnapShot, &hentry));
				}
			} while (Heap32ListNext(hSnapShot, &hlentry));
		}
		CloseToolhelp32Snapshot(hSnapShot);
	}
	SetProcPermissions(orig_perm);
	total_cpu_time /= 10;
	process_cpu_time /= 10;

#else
	 
	if (MyGetSystemTimes) {
		u64 u_time;
		MyGetSystemTimes(&proc_idle_time, &proc_k_u_time, &u_time);
		proc_k_u_time += u_time;
		proc_idle_time /= 10;
		proc_k_u_time /= 10;
	}
	 
	else if (MyQuerySystemInfo) {
		DWORD ret;
		SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION info;
		MyQuerySystemInfo(0x8  , &info, sizeof(info), &ret);
		if (ret && (ret<=sizeof(info))) {
			proc_idle_time = info.IdleTime.QuadPart / 10;
			proc_k_u_time = (info.KernelTime.QuadPart + info.UserTime.QuadPart) / 10;
		}
	}
	 
	else if (flags & GF_RTI_ALL_PROCESSES_TIMES) {
		PROCESSENTRY32 pentry;
		 
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hSnapShot) return GF_FALSE;
		pentry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapShot, &pentry)) {
			do {
				HANDLE procH = NULL;
				if (pentry.th32ProcessID) procH = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pentry.th32ProcessID);
				if (procH && GetProcessTimes(procH, (FILETIME *) &creation, (FILETIME *) &exit, (FILETIME *) &kernel, (FILETIME *) &user) ) {
					user += kernel;
					proc_k_u_time += user;
					if (pentry.th32ProcessID==the_rti.pid) {
						process_k_u_time = user;
					}
				}
				if (procH) CloseHandle(procH);
			} while (Process32Next(hSnapShot, &pentry));
		}
		CloseHandle(hSnapShot);
		proc_k_u_time /= 10;
	}


	if (!process_k_u_time) {
		HANDLE procH = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, the_rti.pid);
		if (procH && GetProcessTimes(procH, (FILETIME *) &creation, (FILETIME *) &exit, (FILETIME *) &kernel, (FILETIME *) &user) ) {
			process_k_u_time = user + kernel;
		}
		if (procH) CloseHandle(procH);
		if (!process_k_u_time) return GF_FALSE;
	}
	process_k_u_time /= 10;

	 
	if (MyGetProcessMemoryInfo) {
		PROCESS_MEMORY_COUNTERS pmc;
		HANDLE procH = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, the_rti.pid);
		MyGetProcessMemoryInfo(procH, &pmc, sizeof (pmc));
		the_rti.process_memory = pmc.WorkingSetSize;
		if (procH) CloseHandle(procH);
	}
	 
	else if (flags & GF_RTI_PROCESS_MEMORY) {
		HEAPLIST32 hlentry;
		HEAPENTRY32 hentry;
		the_rti.process_memory = 0;
		hlentry.dwSize = sizeof(HEAPLIST32);
		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, the_rti.pid);
		if (hSnapShot && Heap32ListFirst(hSnapShot, &hlentry)) {
			do {
				hentry.dwSize = sizeof(hentry);
				if (Heap32First(&hentry, hlentry.th32ProcessID, hlentry.th32HeapID)) {
					do {
						the_rti.process_memory += hentry.dwBlockSize;
					} while (Heap32Next(&hentry));
				}
			} while (Heap32ListNext(hSnapShot, &hlentry));
		}
		CloseHandle(hSnapShot);
	}
#endif

	the_rti.sampling_instant = last_update_time;

	if (last_update_time) {
		the_rti.sampling_period_duration = entry_time - last_update_time;
		the_rti.process_cpu_time_diff = (u32) ((process_k_u_time - last_process_k_u_time)/1000);

#if defined(_WIN32_WCE)
		the_rti.total_cpu_time_diff = (u32) ((total_cpu_time - last_total_k_u_time)/1000);
		 
		if (the_rti.total_cpu_time_diff > the_rti.sampling_period_duration)
			the_rti.sampling_period_duration = the_rti.total_cpu_time_diff;

		 
		the_rti.cpu_idle_time = the_rti.sampling_period_duration - the_rti.total_cpu_time_diff;
		if (!the_rti.sampling_period_duration) the_rti.sampling_period_duration=1;
		the_rti.total_cpu_usage = (u32) (100 * the_rti.total_cpu_time_diff / the_rti.sampling_period_duration);
		if (the_rti.total_cpu_time_diff + the_rti.cpu_idle_time==0) the_rti.total_cpu_time_diff ++;
		the_rti.process_cpu_usage = (u32) (100*the_rti.process_cpu_time_diff / (the_rti.total_cpu_time_diff + the_rti.cpu_idle_time) );

#else
		 
		if (!proc_k_u_time) {
			the_rti.total_cpu_time_diff = the_rti.sampling_period_duration;
			proc_k_u_time = last_proc_k_u_time + the_rti.sampling_period_duration;
			the_rti.cpu_idle_time = 0;
			the_rti.total_cpu_usage = 100;
			if (the_rti.sampling_period_duration)
				the_rti.process_cpu_usage = (u32) (100*the_rti.process_cpu_time_diff / the_rti.sampling_period_duration);
		} else {
			u64 samp_sys_time, idle;
			the_rti.total_cpu_time_diff = (u32) ((proc_k_u_time - last_proc_k_u_time)/1000);

			 
			if (the_rti.total_cpu_time_diff > the_rti.sampling_period_duration) {
				the_rti.sampling_period_duration = the_rti.total_cpu_time_diff;
			}

			if (!proc_idle_time)
				proc_idle_time = last_proc_idle_time + (the_rti.sampling_period_duration - the_rti.total_cpu_time_diff);

			samp_sys_time = proc_k_u_time - last_proc_k_u_time;
			idle = proc_idle_time - last_proc_idle_time;
			the_rti.cpu_idle_time = (u32) (idle/1000);
			if (samp_sys_time) {
				the_rti.total_cpu_usage = (u32) ( (samp_sys_time - idle) / (samp_sys_time / 100) );
				the_rti.process_cpu_usage = (u32) (100*the_rti.process_cpu_time_diff / (samp_sys_time/1000));
			}
		}
#endif
	}
	last_update_time = entry_time;
	last_process_k_u_time = process_k_u_time;

	GlobalMemoryStatus(&ms);
	the_rti.physical_memory = ms.dwTotalPhys;
#ifdef GPAC_MEMORY_TRACKING
	the_rti.gpac_memory = (u64) gpac_allocated_memory;
#endif
	the_rti.physical_memory_avail = ms.dwAvailPhys;

#if defined(_WIN32_WCE)
	last_total_k_u_time = total_cpu_time;
	if (!the_rti.process_memory) the_rti.process_memory = mem_usage_at_startup - ms.dwAvailPhys;
#else
	last_proc_idle_time = proc_idle_time;
	last_proc_k_u_time = proc_k_u_time;
#endif

	if (!the_rti.gpac_memory) the_rti.gpac_memory = the_rti.process_memory;

	memcpy(rti, &the_rti, sizeof(GF_SystemRTInfo));
	return GF_TRUE;
}
