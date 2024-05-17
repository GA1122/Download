CronTab::needsCronTab( ClassAd *ad ) {
	bool ret = false;
	int ctr;
	for ( ctr = 0; ctr < CRONTAB_FIELDS; ctr++ ) {
		if ( ad->LookupExpr( CronTab::attributes[ctr] ) ) {
			ret = true;
			break;
		}
	}  
	return ( ret );
}
