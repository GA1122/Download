BaseShadow::emailUser( const char *subjectline )
{
	dprintf(D_FULLDEBUG, "BaseShadow::emailUser() called.\n");
	if( !jobAd ) {
		return NULL;
	}
	return email_user_open( jobAd, subjectline );
}
