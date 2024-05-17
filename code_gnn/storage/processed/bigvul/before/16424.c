bool CStarter::getJobClaimId(MyString &result)
{
	ClassAd *jobAd = jic ? jic->jobClassAd() : NULL;
	if( jobAd ) {
		return jobAd->LookupString(ATTR_CLAIM_ID,result);
	}
	return false;
}
