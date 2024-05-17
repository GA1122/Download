int getJobAdExitCode(ClassAd *jad, int &exit_code)
{
	if( ! jad->LookupInteger(ATTR_ON_EXIT_CODE, exit_code) ) {
		return FALSE;
	}

	return TRUE;
}
