int getJobAdExitSignal(ClassAd *jad, int &exit_signal)
{
	if( ! jad->LookupInteger(ATTR_ON_EXIT_SIGNAL, exit_signal) ) {
		return FALSE;
	}

	return TRUE;
}
