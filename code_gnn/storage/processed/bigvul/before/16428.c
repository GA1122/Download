CStarter::jobWaitUntilExecuteTime( void )
{
	bool ret = true;
	bool abort = false;
	MyString error;
	
	ClassAd* jobAd = this->jic->jobClassAd();
	int deferralTime = 0;
	int deferralOffset = 0;
	int deltaT = 0;
	int deferralWindow = 0;
	if ( jobAd->LookupExpr( ATTR_DEFERRAL_TIME ) != NULL ) {
		if ( ! jobAd->EvalInteger( ATTR_DEFERRAL_TIME, NULL, deferralTime ) ) {
			error.sprintf( "Invalid deferred execution time for Job %d.%d.",
							this->jic->jobCluster(),
							this->jic->jobProc() );
			abort = true;
		} else if ( deferralTime <= 0 ) {
			error.sprintf( "Invalid execution time '%d' for Job %d.%d.",
							deferralTime,
							this->jic->jobCluster(),
							this->jic->jobProc() );
			abort = true;
 
		} else {
			time_t now = time(NULL);
			if ( jobAd->LookupInteger( ATTR_DEFERRAL_OFFSET, deferralOffset ) ) {
				dprintf( D_FULLDEBUG, "Job %d.%d deferral time offset by "
				                      "%d seconds\n", 
							this->jic->jobCluster(),
							this->jic->jobProc(),
							deferralOffset );
				now -= deferralOffset;
			}
			if ( jobAd->LookupExpr( ATTR_DEFERRAL_WINDOW ) != NULL &&
				 jobAd->EvalInteger( ATTR_DEFERRAL_WINDOW, NULL, deferralWindow ) ) {
				dprintf( D_FULLDEBUG, "Job %d.%d has a deferral window of "
				                      "%d seconds\n", 
							this->jic->jobCluster(),
							this->jic->jobProc(),
							deferralWindow );
			}
			deltaT = deferralTime - now;
			if ( deltaT < 0 ) {
				if ( abs( deltaT ) > deferralWindow ) {
					error.sprintf( "Job %d.%d missed its execution time.",
								this->jic->jobCluster(),
								this->jic->jobProc() );
					abort = true;

				} else {
					dprintf( D_ALWAYS, "Job %d.%d missed its execution time but "
										"is within the %d seconds window\n",
								this->jic->jobCluster(),
								this->jic->jobProc(),
								deferralWindow );
					deltaT = 0;
				}
			}  
		}	
	}
	
	if ( ! abort ) {
		ASSERT( this->deferral_tid == -1 );
		
		this->deferral_tid = daemonCore->Register_Timer(
										deltaT,
										(TimerHandlercpp)&CStarter::SpawnPreScript,
										"deferred job start",
										this );
		if( this->deferral_tid < 0 ) {
			EXCEPT( "Can't register Deferred Execution DaemonCore timer" );
		}
		if ( deltaT > 0 ) { 
			dprintf( D_ALWAYS, "Job %d.%d deferred for %d seconds\n", 
						this->jic->jobCluster(),
						this->jic->jobProc(),
						deltaT );
		} else {
			dprintf( D_ALWAYS, "Job %d.%d set to execute immediately\n",
						this->jic->jobCluster(),
						this->jic->jobProc() );
		}
		
	} else {
		dprintf( D_ALWAYS, "%s Aborting.\n", error.Value() );
		OsProc proc( jobAd );
		proc.JobReaper( -1, JOB_MISSED_DEFERRAL_TIME );
		this->jic->notifyJobExit( -1, JOB_MISSED_DEFERRAL_TIME, &proc );
		this->allJobsDone();
		ret = false;
	}
	
	return ( ret );
}
