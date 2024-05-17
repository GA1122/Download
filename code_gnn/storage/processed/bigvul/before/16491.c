config_fill_ad( ClassAd* ad, const char *prefix )
{
	char 		*tmp;
	char		*expr;
	StringList	reqdExprs;
	MyString 	buffer;

	if( !ad ) return;

	if ( ( NULL == prefix ) && get_mySubSystem()->hasLocalName() ) {
		prefix = get_mySubSystem()->getLocalName();
	}

	buffer.sprintf( "%s_EXPRS", get_mySubSystem()->getName() );
	tmp = param( buffer.Value() );
	if( tmp ) {
		reqdExprs.initializeFromString (tmp);	
		free (tmp);
	}

	buffer.sprintf( "%s_ATTRS", get_mySubSystem()->getName() );
	tmp = param( buffer.Value() );
	if( tmp ) {
		reqdExprs.initializeFromString (tmp);	
		free (tmp);
	}

	if(prefix) {
		buffer.sprintf( "%s_%s_EXPRS", prefix, get_mySubSystem()->getName() );
		tmp = param( buffer.Value() );
		if( tmp ) {
			reqdExprs.initializeFromString (tmp);	
			free (tmp);
		}

		buffer.sprintf( "%s_%s_ATTRS", prefix, get_mySubSystem()->getName() );
		tmp = param( buffer.Value() );
		if( tmp ) {
			reqdExprs.initializeFromString (tmp);	
			free (tmp);
		}

	}

	if( !reqdExprs.isEmpty() ) {
		reqdExprs.rewind();
		while ((tmp = reqdExprs.next())) {
			expr = NULL;
			if(prefix) {
				buffer.sprintf("%s_%s", prefix, tmp);	
				expr = param(buffer.Value());
			}
			if(!expr) {
				expr = param(tmp);
			}
			if(expr == NULL) continue;
			buffer.sprintf( "%s = %s", tmp, expr );

			if( !ad->Insert( buffer.Value() ) ) {
				dprintf(D_ALWAYS,
						"CONFIGURATION PROBLEM: Failed to insert ClassAd attribute %s.  The most common reason for this is that you forgot to quote a string value in the list of attributes being added to the %s ad.\n",
						buffer.Value(), get_mySubSystem()->getName() );
			}

			free( expr );
		}	
	}
	
	 
	ad->Assign( ATTR_VERSION, CondorVersion() );

	ad->Assign( ATTR_PLATFORM, CondorPlatform() );
}
