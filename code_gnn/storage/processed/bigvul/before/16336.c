void BaseShadow::initUserLog()
{
	MyString logfilename;
	int  use_xml;
	bool result;

	ASSERT( job_updater );

	if ( getPathToUserLog(jobAd, logfilename) ) {
		result = uLog.initialize (owner.Value(), domain.Value(), logfilename.Value(), cluster, proc, 0, gjid);
		if ( result == false ) {
			MyString hold_reason;
			hold_reason.sprintf(
				"Failed to initialize user log to %s", logfilename.Value());
			dprintf( D_ALWAYS, "%s\n",hold_reason.Value());
			holdJobAndExit(hold_reason.Value(),CONDOR_HOLD_CODE_UnableToInitUserLog,0);
			EXCEPT("Failed to initialize user log to %s",logfilename.Value());
		}
		if (jobAd->LookupBool(ATTR_ULOG_USE_XML, use_xml)
			&& use_xml) {
			uLog.setUseXML(true);
		} else {
			uLog.setUseXML(false);
		}
		dprintf(D_FULLDEBUG, "%s = %s\n", ATTR_ULOG_FILE, logfilename.Value());
	} else {
		dprintf(D_FULLDEBUG, "no %s found\n", ATTR_ULOG_FILE);
	}
}
