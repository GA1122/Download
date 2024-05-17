mayUserForceRm( )
{
	const char * PARAM_ALLOW_FORCE_RM = "ALLOW_FORCE_RM";
	char* tmp = param( PARAM_ALLOW_FORCE_RM );
	if( tmp == NULL) {
		return true;
	}

	ClassAd tmpAd;
	const char * TESTNAME = "test";
	if(tmpAd.AssignExpr(TESTNAME, tmp) == FALSE) {
		fprintf(stderr, "The configuration setting %s may be invalid.  Treating as TRUE.\n", PARAM_ALLOW_FORCE_RM);
		free(tmp);
		return true;
	}

	free(tmp);

	int is_okay = 0;
	if(tmpAd.EvalBool(TESTNAME, 0, is_okay)) {
		return is_okay;
	} else {
		return true;
	}
}
