write_stats(void)
{
	FILE	*fp;
#ifdef DOSYNCTODR
	struct timeval tv;
#if !defined(VMS)
	int	prio_set;
#endif
#ifdef HAVE_GETCLOCK
	struct timespec ts;
#endif
	int	o_prio;

	 

#if !defined(VMS)
	 
	errno = 0;
	prio_set = 0;
	o_prio = getpriority(PRIO_PROCESS,0);  

	 
	if ((errno == 0) && (setpriority(PRIO_PROCESS,0,-20) == 0))
		prio_set = 1;	 
#endif  
#ifdef HAVE_GETCLOCK
	(void) getclock(TIMEOFDAY, &ts);
	tv.tv_sec = ts.tv_sec;
	tv.tv_usec = ts.tv_nsec / 1000;
#else  
	GETTIMEOFDAY(&tv,(struct timezone *)NULL);
#endif  
	if (ntp_set_tod(&tv,(struct timezone *)NULL) != 0)
		msyslog(LOG_ERR, "can't sync battery time: %m");
#if !defined(VMS)
	if (prio_set)
		setpriority(PRIO_PROCESS, 0, o_prio);  
#endif  
#endif  
	record_sys_stats();
	if (stats_drift_file != 0) {

		 
#ifdef DEBUG
		if (debug)
			printf("write_stats: frequency %.6lf thresh %.6lf, freq %.6lf\n",
			    (prev_drift_comp - drift_comp) * 1e6, wander_resid *
			    1e6, drift_comp * 1e6);
#endif
		if (fabs(prev_drift_comp - drift_comp) < wander_resid) {
			wander_resid *= 0.5;
			return;
		}
		prev_drift_comp = drift_comp;
		wander_resid = wander_threshold;
		if ((fp = fopen(stats_temp_file, "w")) == NULL) {
			msyslog(LOG_ERR, "frequency file %s: %m",
			    stats_temp_file);
			return;
		}
		fprintf(fp, "%.3f\n", drift_comp * 1e6);
		(void)fclose(fp);
		 
#ifdef SYS_WINNT
		if (_unlink(stats_drift_file))  
			msyslog(LOG_WARNING, 
				"Unable to remove prior drift file %s, %m", 
				stats_drift_file);
#endif  

#ifndef NO_RENAME
		if (rename(stats_temp_file, stats_drift_file))
			msyslog(LOG_WARNING, 
				"Unable to rename temp drift file %s to %s, %m", 
				stats_temp_file, stats_drift_file);
#else
		 
		if ((fp = fopen(stats_drift_file, "w")) ==
		    NULL) {
			msyslog(LOG_ERR,
			    "frequency file %s: %m",
			    stats_drift_file);
			return;
		}
#endif

#if defined(VMS)
		 
		{
			$DESCRIPTOR(oldvers,";-1");
			struct dsc$descriptor driftdsc = {
				strlen(stats_drift_file), 0, 0,
				    stats_drift_file };
			while(lib$delete_file(&oldvers,
			    &driftdsc) & 1);
		}
#endif
	}
}
