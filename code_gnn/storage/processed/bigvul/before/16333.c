BaseShadow::handleUpdateJobAd( int sig )
{
	dprintf ( D_FULLDEBUG, "In handleUpdateJobAd, sig %d\n", sig );
	if (!job_updater->retrieveJobUpdates()) {
		dprintf(D_ALWAYS, "Error: Failed to update JobAd\n");
		return -1;
	}

	shadow_user_policy.checkPeriodic();
	return 0;
}
