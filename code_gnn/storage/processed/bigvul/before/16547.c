CronTab::validate( ClassAd *ad, MyString &error ) {
	bool ret = true;
	int ctr;
	for ( ctr = 0; ctr < CRONTAB_FIELDS; ctr++ ) {
		MyString buffer;
		if ( ad->LookupString( CronTab::attributes[ctr], buffer ) ) {
			MyString curError;
			if ( !CronTab::validateParameter( ctr, buffer.Value(), curError ) ) {
				ret = false;
				error += curError;
			}
		}
	}  
	return ( ret );
}
