BaseShadow::~BaseShadow() {
	if (spool) free(spool);
	if (fsDomain) free(fsDomain);
	if (jobAd) FreeJobAd(jobAd);
	if (gjid) free(gjid); 
	if (scheddAddr) free(scheddAddr);
	if( job_updater ) delete job_updater;
}
