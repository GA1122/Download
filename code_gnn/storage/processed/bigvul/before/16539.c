CronTab::expandParameter( int attribute_idx, int min, int max )
{
	MyString *param = this->parameters[attribute_idx];
	ExtArray<int> *list	= this->ranges[attribute_idx];
	
	MyString error;
	if ( ! CronTab::validateParameter(	attribute_idx,
										param->Value(),
										error ) ) {
		dprintf( D_ALWAYS, "%s", error.Value() );
		CronTab::errorLog += error;
		return ( false );
	}
	param->replaceString(" ", "");
	
	param->Tokenize();
	const char *_token;
	while ( ( _token = param->GetNextToken( CRONTAB_DELIMITER, true ) ) != NULL ) {
		MyString token( _token );
		int cur_min = min, cur_max = max, cur_step = 1;
		
		if ( token.find( CRONTAB_STEP ) > 0 ) {
			token.Tokenize();
			const char *_temp;
			const char *_numerator = token.GetNextToken( CRONTAB_STEP, true );
			if ( ( _temp = token.GetNextToken( CRONTAB_STEP, true ) ) != NULL ) {
				MyString stepStr( _temp );
				stepStr.trim();
				cur_step = atoi( stepStr.Value() );
			}
			token = _numerator;
		}  
		
		if ( token.find( CRONTAB_RANGE ) > 0 ) {
			token.Tokenize();
			MyString *_temp;
			int value;
			
			_temp = new MyString( token.GetNextToken( CRONTAB_RANGE, true ) );
			_temp->trim();
			value = atoi( _temp->Value() );
			cur_min = ( value >= min ? value : min );
			delete _temp;
			_temp = new MyString( token.GetNextToken( CRONTAB_RANGE, true ) );
			_temp->trim();
			value = atoi( _temp->Value() );
			cur_max = ( value <= max ? value : max );
			delete _temp;
			
		} else if ( token.find( CRONTAB_WILDCARD ) >= 0 ) {
				if ( attribute_idx  == CRONTAB_DOW_IDX ) {
					continue;
				}
		} else {
			int value = atoi(token.Value());
			if ( value >= min && value <= max ) {
				cur_min = cur_max = value;
			}
		}
		int ctr;
		for ( ctr = cur_min; ctr <= cur_max; ctr++ ) {
			int temp = ctr;
			if ( attribute_idx == CRONTAB_DOW_IDX && 
				 temp == CRONTAB_DAY_OF_WEEK_MAX ) {
				temp = CRONTAB_DAY_OF_WEEK_MIN;
			}
			
			if ( ( ( temp % cur_step ) == 0 ) && !this->contains( *list, temp ) ) {
				list->add( temp );
		 	}
		}  
	}  
	this->sort( *list );	
	return ( true );
}
