CStarter::Reaper(int pid, int exit_status)
{
	int handled_jobs = 0;
	int all_jobs = 0;
	UserProc *job;

	if( WIFSIGNALED(exit_status) ) {
		dprintf( D_ALWAYS, "Process exited, pid=%d, signal=%d\n", pid,
				 WTERMSIG(exit_status) );
	} else {
		dprintf( D_ALWAYS, "Process exited, pid=%d, status=%d\n", pid,
				 WEXITSTATUS(exit_status) );
	}

	if( pre_script && pre_script->JobReaper(pid, exit_status) ) {		
		
		if( ! SpawnJob() ) {
			dprintf( D_ALWAYS, "Failed to start main job, exiting\n" );
			main_shutdown_fast();
			return FALSE;
		}
		return TRUE;
	}

	if( post_script && post_script->JobReaper(pid, exit_status) ) {
		allJobsDone();
		return TRUE;
	}


	m_job_list.Rewind();
	while ((job = m_job_list.Next()) != NULL) {
		all_jobs++;
		if( job->GetJobPid()==pid && job->JobReaper(pid, exit_status) ) {
			handled_jobs++;
			m_job_list.DeleteCurrent();
			m_reaped_job_list.Append(job);
		}
	}

	dprintf( D_FULLDEBUG, "Reaper: all=%d handled=%d ShuttingDown=%d\n",
			 all_jobs, handled_jobs, ShuttingDown );

	if( handled_jobs == 0 ) {
		dprintf( D_ALWAYS, "unhandled job exit: pid=%d, status=%d\n",
				 pid, exit_status );
	}
	if( all_jobs - handled_jobs == 0 ) {
		if( post_script ) {
			post_script->StartJob();
			return TRUE;
		} else {
			if( !allJobsDone() ) {
				dprintf(D_ALWAYS, "Returning from CStarter::JobReaper()\n");
				return 0;
			}
		}
	}

	if ( ShuttingDown && (all_jobs - handled_jobs == 0) ) {
		dprintf(D_ALWAYS,"Last process exited, now Starter is exiting\n");
		StarterExit(0);
	}

	return 0;
}
