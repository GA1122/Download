doWorkByList( StringList* ids, CondorError *errstack )
{
	ClassAd* rval = 0;
	switch( mode ) {
	case JA_RELEASE_JOBS:
		rval = schedd->releaseJobs( ids, actionReason, errstack );
		break;
	case JA_REMOVE_X_JOBS:
		rval = schedd->removeXJobs( ids, actionReason, errstack );
		break;
	case JA_VACATE_JOBS:
		rval = schedd->vacateJobs( ids, VACATE_GRACEFUL, errstack );
		break;
	case JA_VACATE_FAST_JOBS:
		rval = schedd->vacateJobs( ids, VACATE_FAST, errstack );
		break;
	case JA_REMOVE_JOBS:
		rval = schedd->removeJobs( ids, actionReason, errstack );
		break;
	case JA_HOLD_JOBS:
		rval = schedd->holdJobs( ids, actionReason, holdReasonSubCode, errstack );
		break;
	case JA_SUSPEND_JOBS:
		rval = schedd->suspendJobs( ids, actionReason, errstack );
		break;
	case JA_CONTINUE_JOBS:
		rval = schedd->continueJobs( ids, actionReason, errstack );
		break;
	default:
		EXCEPT( "impossible: unknown mode in doWorkByList" );
	}
	if( ! rval ) {
		had_error = true;
	} else {
		int result = FALSE;
		if( !rval->LookupInteger(ATTR_ACTION_RESULT, result) || !result ) {
			had_error = true;
		}
	}
	return rval;
}
