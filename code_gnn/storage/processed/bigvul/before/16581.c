GetDesiredDelegatedJobCredentialExpiration(ClassAd *job)
{
	if ( !param_boolean( "DELEGATE_JOB_GSI_CREDENTIALS", true ) ) {
		return 0;
	}

	time_t expiration_time = 0;
	int lifetime = 0;
	if( job ) {
		job->LookupInteger(ATTR_DELEGATE_JOB_GSI_CREDENTIALS_LIFETIME,lifetime);
	}
	if( !lifetime ) {
		lifetime = param_integer("DELEGATE_JOB_GSI_CREDENTIALS_LIFETIME",3600*24);
	}
	if( lifetime ) {
		expiration_time = time(NULL) + lifetime;
	}
	return expiration_time;
}
