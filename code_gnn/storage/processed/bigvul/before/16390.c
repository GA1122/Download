CStarter::PeriodicCkpt( void )
{
	dprintf(D_ALWAYS, "Periodic Checkpointing all jobs.\n");

	if( jobUniverse != CONDOR_UNIVERSE_VM ) {
		return false;
	}

	UserProc *job;
	m_job_list.Rewind();
	while ((job = m_job_list.Next()) != NULL) {
		if( job->Ckpt() ) {

			CondorPrivSepHelper* cpsh = condorPrivSepHelper();
			if (cpsh != NULL) {
				PrivSepError err;
				if( !cpsh->chown_sandbox_to_condor(err) ) {
					jic->notifyStarterError(
						err.holdReason(),
						false,
						err.holdCode(),
						err.holdSubCode());
					dprintf(D_ALWAYS,"failed to change sandbox to condor ownership before checkpoint");
					return false;
				}
			}

			bool transfer_ok = jic->uploadWorkingFiles();

			if (cpsh != NULL) {
				PrivSepError err;
				if( !cpsh->chown_sandbox_to_user(err) ) {
					jic->notifyStarterError(
						err.holdReason(),
						true,
						err.holdCode(),
						err.holdSubCode());
					EXCEPT("failed to restore sandbox to user ownership after checkpoint");
					return false;
				}
			}

			if( transfer_ok == false ) {
				dprintf(D_ALWAYS, "Periodic Checkpointing failed.\n");

				job->CkptDone(false);
			}else {
				dprintf(D_ALWAYS, "Periodic Checkpointing is done.\n");

				jic->updateCkptInfo();	

				job->CkptDone(true);

			}
		}
	}

	return true;
}
