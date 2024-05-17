CronTab::CronTab(	int minutes,
					int hours,
					int days_of_month,
					int months,
					int days_of_week ) {
	if ( minutes == CRONTAB_CRONOS_STAR ) {
		this->parameters[CRONTAB_MINUTES_IDX] = new MyString( CRONTAB_WILDCARD );
	} else {
		this->parameters[CRONTAB_MINUTES_IDX] = new MyString( minutes );
	}
	if ( hours == CRONTAB_CRONOS_STAR ) {
		this->parameters[CRONTAB_HOURS_IDX]	= new MyString( CRONTAB_WILDCARD );
	} else {
		this->parameters[CRONTAB_HOURS_IDX]	= new MyString( hours );
	}
	if ( days_of_month == CRONTAB_CRONOS_STAR ) {
		this->parameters[CRONTAB_DOM_IDX] = new MyString( CRONTAB_WILDCARD );
	} else {
		this->parameters[CRONTAB_DOM_IDX] = new MyString( days_of_month );
	}
	if ( months == CRONTAB_CRONOS_STAR ) {
		this->parameters[CRONTAB_MONTHS_IDX] = new MyString( CRONTAB_WILDCARD );
	} else {
		this->parameters[CRONTAB_MONTHS_IDX] = new MyString( months );
	}
	if ( days_of_week == CRONTAB_CRONOS_STAR ) {
		this->parameters[CRONTAB_DOW_IDX] = new MyString( CRONTAB_WILDCARD );
	} else {
		this->parameters[CRONTAB_DOW_IDX] = new MyString( days_of_week );
	}
	this->init();
}
