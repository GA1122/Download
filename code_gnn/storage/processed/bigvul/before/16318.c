int BaseShadow::cdToIwd() {
	int iRet =0;
	
#if ! defined(WIN32)
	priv_state p = PRIV_UNKNOWN;
	
	if (m_RunAsNobody)
		p = set_root_priv();
#endif
	
	if (chdir(iwd.Value()) < 0) {
		int chdir_errno = errno;
		dprintf(D_ALWAYS, "\n\nPath does not exist.\n"
				"He who travels without bounds\n"
				"Can't locate data.\n\n" );
		MyString hold_reason;
		hold_reason.sprintf("Cannot access initial working directory %s: %s",
		                    iwd.Value(), strerror(chdir_errno));
		dprintf( D_ALWAYS, "%s\n",hold_reason.Value());
		holdJobAndExit(hold_reason.Value(),CONDOR_HOLD_CODE_IwdError,chdir_errno);
		iRet = -1;
	}
	
#if ! defined(WIN32)
	if ( m_RunAsNobody )
		set_priv(p);
#endif
	
	return iRet;
}
