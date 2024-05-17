int GahpClient::ec2_attach_volume(const char * service_url,
                              const char * publickeyfile,
                              const char * privatekeyfile,
                              const char * volume_id,
							  const char * instance_id, 
                              const char * device_id,
                              StringList & returnStatus,
                              char* & error_code )
{
    static const char* command = "EC_VM_ATTACH_VOLUME";

    int rc=0;
    
    if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
        return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
    }

    if ( (service_url == NULL) || (publickeyfile == NULL) || (privatekeyfile == NULL) || (instance_id == NULL) || (volume_id == NULL) || (device_id == NULL) ){
        return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
    }

    std::string reqline;

    char* esc1 = strdup( escapeGahpString(service_url) );
    char* esc2 = strdup( escapeGahpString(publickeyfile) );
    char* esc3 = strdup( escapeGahpString(privatekeyfile) );
    char* esc4 = strdup( escapeGahpString(volume_id) );
	char* esc5 = strdup( escapeGahpString(instance_id) );
    char* esc6 = strdup( escapeGahpString(device_id) );
    
    int x = sprintf(reqline, "%s %s %s %s %s %s", esc1, esc2, esc3, esc4, esc5, esc6 );
    
    free( esc1 );
    free( esc2 );
    free( esc3 );
    free( esc4 );
    free( esc5 );
	free( esc6 );
    ASSERT( x > 0 );
    
    const char *buf = reqline.c_str();
        
    
    if ( m_mode == results_only ) 
	{
		return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
	}
	else
	{
        now_pending(command, buf, deleg_proxy);
	}
    
    Gahp_Args* result = get_pending_result(command, buf);

    if ( result ) {
        int result_code = atoi(result->argv[1]);
        
        if (result_code == 1) {
            
            if (result->argc == 2) {
                error_string = "";
            } else if (result->argc == 4) {
                error_code = strdup(result->argv[2]);
                error_string = result->argv[3];
            } else {
                EXCEPT("Bad %s Result",command);
            }
            
        } else {     
            
            if ( ( (result->argc-2) % 2) != 0 ) {
                EXCEPT("Bad %s Result",command);
            } else {
                for (int i=2; i<result->argc; i++) {
                    returnStatus.append( strdup(result->argv[i]) );
                }
                returnStatus.rewind();
            }
        }       
        
        delete result;
        
    }
    
    return rc;

}
