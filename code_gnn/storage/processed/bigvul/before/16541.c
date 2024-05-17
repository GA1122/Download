CronTab::init() {
	initRegexObject();
	this->lastRunTime = CRONTAB_INVALID;
	this->valid = false;
	
	int mins[]				= { CRONTAB_MINUTE_MIN,
								CRONTAB_HOUR_MIN,
								CRONTAB_DAY_OF_MONTH_MIN,
								CRONTAB_MONTH_MIN,
								CRONTAB_DAY_OF_WEEK_MIN
	};
	int maxs[]				= { CRONTAB_MINUTE_MAX,
								CRONTAB_HOUR_MAX,
								CRONTAB_DAY_OF_MONTH_MAX,
								CRONTAB_MONTH_MAX,
								CRONTAB_DAY_OF_WEEK_MAX
	};
	bool failed = false;
	int ctr;
	for ( ctr = 0; ctr < CRONTAB_FIELDS; ctr++ ) {
		this->ranges[ctr] = new ExtArray<int>();			
		if ( !this->expandParameter( ctr, mins[ctr], maxs[ctr] )) {
			failed = true;
		}
	}  
	if ( !failed ) {
		this->valid = true;
	}
	
	return;
}
