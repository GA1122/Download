CStarter::PublishToEnv( Env* proc_env )
{
	ASSERT(proc_env);
	if( pre_script ) {
		pre_script->PublishToEnv( proc_env );
	}

	proc_env->SetEnv("_CONDOR_JOB_IWD",jic->jobRemoteIWD());

	MyString job_pids;
	UserProc* uproc;
	m_job_list.Rewind();
	while ((uproc = m_job_list.Next()) != NULL) {
		uproc->PublishToEnv( proc_env );

		if( ! job_pids.IsEmpty() ) {
			job_pids += " ";
		}
		job_pids.sprintf_cat("%d",uproc->GetJobPid());		
	}
	proc_env->SetEnv("_CONDOR_JOB_PIDS",job_pids);

	m_reaped_job_list.Rewind();
	while ((uproc = m_reaped_job_list.Next()) != NULL) {
		uproc->PublishToEnv( proc_env );
	}

	ASSERT(jic);
	ClassAd* jobAd = jic->jobClassAd();
	if( jobAd ) {
		bool using_file_transfer = jic->usingFileTransfer();
		build_job_env( *proc_env, *jobAd, using_file_transfer );
	} else {
		dprintf(D_ALWAYS, "Unable to find job ad for job.  Environment may be incorrect\n");
	}

	MyString base;
	base = "_";
	base += myDistro->GetUc();
	base += '_';
 
	MyString env_name;

	const char* output_ad = jic->getOutputAdFile();
	if( output_ad && !(output_ad[0] == '-' && output_ad[1] == '\0') ) {
		env_name = base.Value();
		env_name += "OUTPUT_CLASSAD";
		proc_env->SetEnv( env_name.Value(), output_ad );
	}
	
	env_name = base.Value();
	env_name += "SCRATCH_DIR";
	proc_env->SetEnv( env_name.Value(), GetWorkingDir() );

	env_name = base.Value();
	env_name += "SLOT";
	int slot = getMySlotNumber();
	if (!slot) {
		slot = 1;
	}
	proc_env->SetEnv(env_name.Value(), slot);


	int low, high;
	if (get_port_range (TRUE, &low, &high) == TRUE) {
		MyString tmp_port_number;

		tmp_port_number = high;
		env_name = base.Value();
		env_name += "HIGHPORT";
		proc_env->SetEnv( env_name.Value(), tmp_port_number.Value() );

		tmp_port_number = low;
		env_name = base.Value();
		env_name += "LOWPORT";
		proc_env->SetEnv( env_name.Value(), tmp_port_number.Value() );
    }


	proc_env->SetEnv("TMPDIR", GetWorkingDir());
	proc_env->SetEnv("TEMP", GetWorkingDir());  
	proc_env->SetEnv("TMP", GetWorkingDir());  
}
