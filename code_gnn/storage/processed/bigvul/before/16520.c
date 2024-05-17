param_without_default( const char *name )
{
	char		*val = NULL;
	char param_name[MAX_PARAM_LEN];


	const char	*local = get_mySubSystem()->getLocalName();
	if (  (NULL == val) && local ) {
		snprintf(param_name,MAX_PARAM_LEN,"%s.%s.%s",
				 get_mySubSystem()->getName(),
				 local,
				 name);
		param_name[MAX_PARAM_LEN-1]='\0';
		strlwr(param_name);
		val = lookup_macro_lower( param_name, ConfigTab, TABLESIZE );
	}
	if (  (NULL == val) && local ) {
		snprintf(param_name,MAX_PARAM_LEN,"%s.%s",
				 local,
				 name);
		param_name[MAX_PARAM_LEN-1]='\0';
		strlwr(param_name);
		val = lookup_macro_lower( param_name, ConfigTab, TABLESIZE );
	}
	if ( NULL == val ) {
		snprintf(param_name,MAX_PARAM_LEN,"%s.%s",
				 get_mySubSystem()->getName(),
				 name);
		param_name[MAX_PARAM_LEN-1]='\0';
		strlwr(param_name);
		val = lookup_macro_lower( param_name, ConfigTab, TABLESIZE );
	}
	if ( NULL == val ) {
		snprintf(param_name,MAX_PARAM_LEN,"%s",name);
		param_name[MAX_PARAM_LEN-1]='\0';
		strlwr(param_name);
		val = lookup_macro_lower( param_name, ConfigTab, TABLESIZE );
	}

	if ( (NULL == val) || (*val=='\0') ) {
		return NULL;
	}

	if( DebugFlags & D_CONFIG ) {
		if( strlen(name) < strlen(param_name) ) {
			param_name[strlen(param_name)-strlen(name)] = '\0';
			dprintf( D_CONFIG, "Config '%s': using prefix '%s' ==> '%s'\n",
					 name, param_name, val );
		}
		else {
			dprintf( D_CONFIG, "Config '%s': no prefix ==> '%s'\n", name, val );
		}
	}

	val = expand_macro( val, ConfigTab, TABLESIZE );

	if( val == NULL ) {
		return NULL;
	} else if ( val[0] == '\0' ) {
		free( val );
		return( NULL );
	} else {
		return val;
	}
}
