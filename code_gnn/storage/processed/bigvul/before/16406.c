CStarter::SpawnJob( void )
{
	ClassAd* jobAd = jic->jobClassAd();
	if ( jobAd->LookupInteger( ATTR_JOB_UNIVERSE, jobUniverse ) < 1 ) {
		dprintf( D_ALWAYS, 
				 "Job doesn't specify universe, assuming VANILLA\n" ); 
	}
	dprintf( D_ALWAYS, "Starting a %s universe job with ID: %d.%d\n",
			 CondorUniverseName(jobUniverse), jic->jobCluster(),
			 jic->jobProc() );

	UserProc *job;
	switch ( jobUniverse )  
	{
		case CONDOR_UNIVERSE_LOCAL:
		case CONDOR_UNIVERSE_VANILLA:
			job = new VanillaProc( jobAd );
			break;
		case CONDOR_UNIVERSE_JAVA:
			job = new JavaProc( jobAd, WorkingDir.Value() );
			break;
	    case CONDOR_UNIVERSE_PARALLEL:
			job = new ParallelProc( jobAd );
			break;
		case CONDOR_UNIVERSE_MPI: {
			int is_master = FALSE;
			if ( jobAd->LookupBool( ATTR_MPI_IS_MASTER, is_master ) < 1 ) {
				is_master = FALSE;
			}
			if ( is_master ) {
				dprintf ( D_FULLDEBUG, "Starting a MPIMasterProc\n" );
				job = new MPIMasterProc( jobAd );
			} else {
				dprintf ( D_FULLDEBUG, "Starting a MPIComradeProc\n" );
				job = new MPIComradeProc( jobAd );
			}
			break;
		}
		case CONDOR_UNIVERSE_VM:
			job = new VMProc( jobAd );
			ASSERT(job);
			break;
		default:
			dprintf( D_ALWAYS, "Starter doesn't support universe %d (%s)\n",
					 jobUniverse, CondorUniverseName(jobUniverse) ); 
			return FALSE;
	}  

	if (job->StartJob()) {
		m_job_list.Append(job);
		
		if ( this->suspended ) {
			this->Suspend( );
		}

		char* tool_daemon_name = NULL;
		jobAd->LookupString( ATTR_TOOL_DAEMON_CMD,
							 &tool_daemon_name );
		if( tool_daemon_name ) {
			ToolDaemonProc* tool_daemon_proc;
			tool_daemon_proc = new ToolDaemonProc( jobAd, job->GetJobPid() );

			if( tool_daemon_proc->StartJob() ) {
				m_job_list.Append( tool_daemon_proc );
				dprintf( D_FULLDEBUG, "ToolDaemonProc added to m_job_list\n");
				if ( this->suspended ) {
					this->Suspend( );
				}
			} else {
				dprintf( D_ALWAYS, "Failed to start ToolDaemonProc!\n");
				delete tool_daemon_proc;
			}
			free( tool_daemon_name );
		}

		jic->allJobsSpawned();
		return TRUE;
	} else {
		delete job;
		dprintf( D_ALWAYS, "Failed to start job, exiting\n" );
		main_shutdown_fast();
		return FALSE;
	}
}
