void CStarter::getJobOwnerFQUOrDummy(MyString &result)
{
	ClassAd *jobAd = jic ? jic->jobClassAd() : NULL;
	if( jobAd ) {
		jobAd->LookupString(ATTR_USER,result);
	}
	if( result.IsEmpty() ) {
		result = "job-owner@submit-domain";
	}
}
