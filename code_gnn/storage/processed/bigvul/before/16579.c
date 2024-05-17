GetDelegatedProxyRenewalTime(time_t expiration_time)
{
	if( expiration_time == 0 ) {
		return 0;
	}
	if ( !param_boolean( "DELEGATE_JOB_GSI_CREDENTIALS", true ) ) {
		return 0;
	}

	time_t now = time(NULL);
	time_t lifetime = expiration_time - now;
	double lifetime_frac = param_double( "DELEGATE_JOB_GSI_CREDENTIALS_RENEWAL", 0.25,0,1);
	return now + (time_t)floor(lifetime*lifetime_frac);
}
