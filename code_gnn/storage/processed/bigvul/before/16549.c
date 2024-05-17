CronTab::~CronTab() {
	int ctr;
	for ( ctr = 0; ctr < CRONTAB_FIELDS; ctr++ ) {
		if ( this->ranges[ctr] )		delete this->ranges[ctr];
		if ( this->parameters[ctr] )	delete this->parameters[ctr];
	}  
}
