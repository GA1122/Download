proc_exists( int cluster, int proc )
{
	ClassAd *ad;

	if ((ad = GetJobAd(cluster,proc)) != NULL) {
		FreeJobAd(ad);
		return TRUE;
	}

	return FALSE;
}
