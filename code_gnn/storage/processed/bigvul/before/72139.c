static int _run_job_script(const char *name, const char *path,
			   uint32_t jobid, int timeout, char **env, uid_t uid)
{
	struct stat stat_buf;
	int status = 0, rc;

	 
	if (conf->plugstack && (stat(conf->plugstack, &stat_buf) == 0))
		status = _run_spank_job_script(name, env, jobid, uid);
	if ((rc = run_script(name, path, jobid, timeout, env, uid)))
		status = rc;
	return (status);
}