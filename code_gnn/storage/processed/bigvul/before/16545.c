CronTab::nextRunTime( long timestamp ) {
	long runtime = CRONTAB_INVALID;
	struct tm *tm;

	if ( ! this->valid ) {
		this->lastRunTime = CRONTAB_INVALID;
		return ( this->lastRunTime );
	}
		
	timestamp += ( 60 - ( timestamp % 60 ) );
	const time_t _timestamp = (time_t)timestamp;
		
	tm = localtime( &_timestamp );
	int fields[CRONTAB_FIELDS];
	fields[CRONTAB_MINUTES_IDX]	= tm->tm_min;
	fields[CRONTAB_HOURS_IDX]	= tm->tm_hour;
	fields[CRONTAB_DOM_IDX]		= tm->tm_mday;
	fields[CRONTAB_MONTHS_IDX]	= tm->tm_mon + 1;
	fields[CRONTAB_DOW_IDX]		= tm->tm_wday;
	
	int match[CRONTAB_FIELDS + 1];
	match[CRONTAB_YEARS_IDX] = tm->tm_year + 1900;
	match[CRONTAB_DOW_IDX] = -1;
	
	if ( this->matchFields( fields, match, CRONTAB_FIELDS - 2 ) ) {
		struct tm matchTime;
		matchTime.tm_sec	= 0;
		matchTime.tm_min	= match[CRONTAB_MINUTES_IDX];
		matchTime.tm_hour	= match[CRONTAB_HOURS_IDX];
		matchTime.tm_mday	= match[CRONTAB_DOM_IDX];
		matchTime.tm_mon	= match[CRONTAB_MONTHS_IDX] - 1;
		matchTime.tm_year	= match[CRONTAB_YEARS_IDX] - 1900;
		matchTime.tm_isdst  = -1;  
		runtime = (long)mktime( &matchTime );
		
		if ( runtime < timestamp ) {
			EXCEPT( "CronTab: Generated a runtime that is in the past (%d < %d)"
				, (int)runtime, (int)timestamp );
		}
		
	} else {
		EXCEPT( "CronTab: Failed to find a match for timestamp %d", 
			(int)timestamp );
	}
	
	this->lastRunTime = runtime;
	return ( runtime );
}
