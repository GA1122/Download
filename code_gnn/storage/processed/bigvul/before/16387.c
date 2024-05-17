CStarter::GetJobEnv( ClassAd *jobad, Env *job_env, MyString *env_errors )
{
	char *env_str = param( "STARTER_JOB_ENVIRONMENT" );

	ASSERT( jobad );
	ASSERT( job_env );
	if( !job_env->MergeFromV1RawOrV2Quoted(env_str,env_errors) ) {
		if( env_errors ) {
			env_errors->sprintf_cat(
				" The full value for STARTER_JOB_ENVIRONMENT: %s\n",env_str);
		}
		free(env_str);
		return false;
	}
	free(env_str);

	if(!job_env->MergeFrom(jobad,env_errors)) {
		if( env_errors ) {
			env_errors->sprintf_cat(
				" (This error was from the environment string in the job "
				"ClassAd.)");
		}
		return false;
	}

	PublishToEnv( job_env );
	return true;
}	
