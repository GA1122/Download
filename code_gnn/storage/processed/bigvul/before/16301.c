doWorkByConstraint( const char* constraint, CondorError * errstack )
{
	ClassAd* ad = 0;
	int total_jobs = -1;
	bool rval = true;
	switch( mode ) {
	case JA_RELEASE_JOBS:
		ad = schedd->releaseJobs( constraint, actionReason, errstack );
		break;
	case JA_REMOVE_X_JOBS:
		ad = schedd->removeXJobs( constraint, actionReason, errstack );
		break;
	case JA_VACATE_JOBS:
		ad = schedd->vacateJobs( constraint, VACATE_GRACEFUL, errstack );
		break;
	case JA_VACATE_FAST_JOBS:
		ad = schedd->vacateJobs( constraint, VACATE_FAST, errstack );
		break;
	case JA_REMOVE_JOBS:
		ad = schedd->removeJobs( constraint, actionReason, errstack );
		break;
	case JA_HOLD_JOBS:
		ad = schedd->holdJobs( constraint, actionReason, holdReasonSubCode, errstack );
		break;
	case JA_SUSPEND_JOBS:
		ad = schedd->suspendJobs( constraint, actionReason, errstack );
		break;
	case JA_CONTINUE_JOBS:
		ad = schedd->continueJobs( constraint, actionReason, errstack );
		break;
	default:
		EXCEPT( "impossible: unknown mode in doWorkByConstraint" );
	}
	if( ! ad ) {
		had_error = true;
		rval = false;
	} else {
		int result = FALSE;
		if( !ad->LookupInteger(ATTR_ACTION_RESULT, result) ) {
			had_error = true;
			rval = false;
		}
		else if( !result ) {
		        if( !ad->LookupInteger(ATTR_TOTAL_JOB_ADS, total_jobs) || total_jobs > 0 ) {
				had_error = true;
			} else {
				errstack->push("condor_rm", 0, "There are no jobs in the queue");
			}
			rval = false;
		}
	}
	return rval;
}
