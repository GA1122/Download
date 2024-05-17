actionWord( JobAction action, bool past )
{
	switch( action ) {
	case JA_RELEASE_JOBS:
		return past ? "released" : "release";
		break;

	case JA_HOLD_JOBS:
		return past ? "held" : "hold";
		break;
	
	case JA_SUSPEND_JOBS:
		return past ? "suspended" : "suspend";
		break;
		
	case JA_CONTINUE_JOBS:
		return past ? "continued" : "continue";
		break;

	case JA_REMOVE_JOBS:
	case JA_REMOVE_X_JOBS:
		return past ? "removed" : "remove";
		break;

	case JA_VACATE_JOBS:
		return past ? "vacated" : "vacate";
		break;

	case JA_VACATE_FAST_JOBS:
		return past ? "fast-vacated" : "fast-vacate";
		break;

	default:
		fprintf( stderr, "ERROR: Unknown action: %d\n", action );
		exit( 1 );
		break;
	}
	return NULL;
}
