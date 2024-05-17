clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	FILETIME ft;
	ULARGE_INTEGER li, li2;
	BOOL ok = FALSE;
	double d;
	static double perfcnt_per_sec = 0.0;
	static BOOL initialized = FALSE;

	if (!initialized) {
		QueryPerformanceFrequency((LARGE_INTEGER *)&li);
		perfcnt_per_sec = 1.0 / li.QuadPart;
		initialized = TRUE;
	}

	if (tp) {
		memset(tp, 0, sizeof(*tp));

		if (clk_id == CLOCK_REALTIME) {

			 
			GetSystemTimeAsFileTime(&ft);
			li.LowPart = ft.dwLowDateTime;
			li.HighPart = ft.dwHighDateTime;
			li.QuadPart -= 116444736000000000;  
			tp->tv_sec = (time_t)(li.QuadPart / 10000000);
			tp->tv_nsec = (long)(li.QuadPart % 10000000) * 100;
			ok = TRUE;
			 

		} else if (clk_id == CLOCK_MONOTONIC) {

			 
			QueryPerformanceCounter((LARGE_INTEGER *)&li);
			d = li.QuadPart * perfcnt_per_sec;
			tp->tv_sec = (time_t)d;
			d -= (double)tp->tv_sec;
			tp->tv_nsec = (long)(d * 1.0E9);
			ok = TRUE;
			 

		} else if (clk_id == CLOCK_THREAD) {

			 
			FILETIME t_create, t_exit, t_kernel, t_user;
			if (GetThreadTimes(GetCurrentThread(),
			                   &t_create,
			                   &t_exit,
			                   &t_kernel,
			                   &t_user)) {
				li.LowPart = t_user.dwLowDateTime;
				li.HighPart = t_user.dwHighDateTime;
				li2.LowPart = t_kernel.dwLowDateTime;
				li2.HighPart = t_kernel.dwHighDateTime;
				li.QuadPart += li2.QuadPart;
				tp->tv_sec = (time_t)(li.QuadPart / 10000000);
				tp->tv_nsec = (long)(li.QuadPart % 10000000) * 100;
				ok = TRUE;
			}
			 

		} else if (clk_id == CLOCK_PROCESS) {

			 
			FILETIME t_create, t_exit, t_kernel, t_user;
			if (GetProcessTimes(GetCurrentProcess(),
			                    &t_create,
			                    &t_exit,
			                    &t_kernel,
			                    &t_user)) {
				li.LowPart = t_user.dwLowDateTime;
				li.HighPart = t_user.dwHighDateTime;
				li2.LowPart = t_kernel.dwLowDateTime;
				li2.HighPart = t_kernel.dwHighDateTime;
				li.QuadPart += li2.QuadPart;
				tp->tv_sec = (time_t)(li.QuadPart / 10000000);
				tp->tv_nsec = (long)(li.QuadPart % 10000000) * 100;
				ok = TRUE;
			}
			 

		} else {

			 
			 
			 
		}
	}

	return ok ? 0 : -1;
}
