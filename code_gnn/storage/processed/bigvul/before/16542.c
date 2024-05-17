CronTab::matchFields( int *curTime, int *match, int attribute_idx, bool useFirst )
{
	bool nextUseFirst = useFirst;
	match[attribute_idx] = -1;
	
	ExtArray<int> *curRange = NULL;
	if ( attribute_idx == CRONTAB_DOM_IDX ) {
			
		if (this->ranges[attribute_idx]->length()==CRONTAB_DAY_OF_MONTH_MAX){	
			if ((this->ranges[CRONTAB_DOW_IDX]->length()==CRONTAB_DAY_OF_WEEK_MAX)||
			   (this->ranges[CRONTAB_DOW_IDX]->length()==0)){ 
				curRange = new ExtArray<int>( *this->ranges[attribute_idx] );
			} else {
				curRange = new ExtArray<int>( CRONTAB_DAY_OF_MONTH_MAX );
			}
		}else{
		      curRange = new ExtArray<int>( *this->ranges[attribute_idx] );
		}
		
		int firstDay = dayOfWeek( match[CRONTAB_MONTHS_IDX],
								  1,
								  match[CRONTAB_YEARS_IDX] );
		int ctr, cnt;
		for ( ctr = 0, cnt = this->ranges[CRONTAB_DOW_IDX]->getlast();
			  ctr <= cnt;
			  ctr++ ) {
			int day = (this->ranges[CRONTAB_DOW_IDX]->getElementAt(ctr) - firstDay) + 1;
			while ( day <= CRONTAB_DAY_OF_MONTH_MAX ) {
				if (curRange && day > 0 && !this->contains( *curRange, day ) ) {
					curRange->add( day );
				}
				day += 7;
			}  
		}  
		this->sort( *curRange );
		
	} else {
		curRange = this->ranges[attribute_idx];
	}

	bool ret = false;
	int range_idx, cnt;
	for ( range_idx = 0, cnt = curRange->getlast();
		  range_idx <= cnt;
		  range_idx++ ) {
		int value = curRange->getElementAt( range_idx );
		if ( useFirst || value >= curTime[attribute_idx] ) {
			if ( value > curTime[attribute_idx] ) nextUseFirst = true;
			if ( attribute_idx == CRONTAB_DOM_IDX ) {
				int maxDOM = daysInMonth( 	match[CRONTAB_MONTHS_IDX],
											match[CRONTAB_YEARS_IDX] );
				if ( value > maxDOM ) {
					continue;
				}
			}
			match[attribute_idx] = value;
			if ( attribute_idx == CRONTAB_MINUTES_IDX ) {
				ret = true;
				break;
				
			} else {
				ret = this->matchFields( curTime, match, attribute_idx - 1, nextUseFirst );
				nextUseFirst = true;
				if ( ret ) break;
			}
		}
	}  
	if ( !ret && attribute_idx == CRONTAB_MONTHS_IDX ) {
		match[CRONTAB_YEARS_IDX]++;	
		ret = this->matchFields( curTime, match, attribute_idx, true );
	}
	
	if ( attribute_idx == CRONTAB_DOM_IDX && curRange ) delete curRange;
	
	return ( ret );
}
