CStarter::getMySlotNumber( void )
{
	
	char *logappend = param("STARTER_LOG");		
	char const *tmp = NULL;
		
	int slot_number = 0;  
			
	if ( logappend ) {
		char const *log_basename = condor_basename(logappend);
		MyString prefix;

		char* resource_prefix = param("STARTD_RESOURCE_PREFIX");
		if( resource_prefix ) {
			prefix.sprintf(".%s",resource_prefix);
			free( resource_prefix );
		}
		else {
			prefix = ".slot";
		}

		tmp = strstr(log_basename, prefix.Value());
		if ( tmp ) {				
			prefix += "%d";
			if ( sscanf(tmp, prefix.Value(), &slot_number) < 1 ) {
				slot_number = 0;
			}
		} 

		free(logappend);
	}

	return slot_number;
}
