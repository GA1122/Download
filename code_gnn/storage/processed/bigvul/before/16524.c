process_locals( const char* param_name, const char* host )
{
	StringList sources_to_process, sources_done;
	char *source, *sources_value;
	int local_required;

	local_required = param_boolean_crufty("REQUIRE_LOCAL_CONFIG_FILE", true);

	sources_value = param( param_name );
	if( sources_value ) {
		if ( is_piped_command( sources_value ) ) {
			sources_to_process.insert( sources_value );
		} else {
			sources_to_process.initializeFromString( sources_value );
		}
		sources_to_process.rewind();
		while( (source = sources_to_process.next()) ) {
			process_config_source( source, "config source", host,
								   local_required );
			local_config_sources.append( source );

			sources_done.append(source);

			char* new_sources_value = param(param_name);
			if(new_sources_value) {
				if(strcmp(sources_value, new_sources_value) ) {
					sources_to_process.clearAll();
					if ( is_piped_command( new_sources_value ) ) {
						sources_to_process.insert( new_sources_value );
					} else {
						sources_to_process.initializeFromString(new_sources_value);
					}

                	sources_done.rewind();
                	while( (source = sources_done.next()) ) {
						sources_to_process.remove(source);
					}
					sources_to_process.rewind();
					free(sources_value);
					sources_value = new_sources_value;
				} else {
					free(new_sources_value);
				}
			}
		}
		free(sources_value);
	}
}
