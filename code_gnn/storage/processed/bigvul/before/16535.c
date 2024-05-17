CronTab::CronTab( ClassAd *ad )
{
	for ( int ctr = 0; ctr < CRONTAB_FIELDS; ctr++ ) {
		MyString buffer;
		if ( ad->LookupString( this->attributes[ctr], buffer ) ) {
			dprintf( D_FULLDEBUG, "CronTab: Pulled out '%s' for %s\n",
						buffer.Value(), this->attributes[ctr] );
			this->parameters[ctr] = new MyString( buffer.Value() );
		} else {
			dprintf( D_FULLDEBUG, "CronTab: No attribute for %s, using wildcard\n",
							this->attributes[ctr] );
			this->parameters[ctr] = new MyString( CRONTAB_WILDCARD );
		}
	}  
	this->init();
}
