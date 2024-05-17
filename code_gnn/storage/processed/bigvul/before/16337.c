BaseShadow::jobWantsGracefulRemoval()
{
	bool job_wants_graceful_removal = false;
	ClassAd *thejobAd = getJobAd();
	if( thejobAd ) {
		thejobAd->LookupBool( ATTR_WANT_GRACEFUL_REMOVAL, job_wants_graceful_removal );
	}
	return job_wants_graceful_removal;
}
