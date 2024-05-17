CronTab::CronTab(	const char* minutes,
					const char* hours,
					const char* days_of_month,
					const char* months,
					const char* days_of_week ) {
	this->parameters[CRONTAB_MINUTES_IDX]	= new MyString( minutes );
	this->parameters[CRONTAB_HOURS_IDX]		= new MyString( hours );
	this->parameters[CRONTAB_DOM_IDX]		= new MyString( days_of_month );
	this->parameters[CRONTAB_MONTHS_IDX]	= new MyString( months );
	this->parameters[CRONTAB_DOW_IDX]		= new MyString( days_of_week );

	this->init();
}
