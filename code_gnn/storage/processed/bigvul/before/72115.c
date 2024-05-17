_rpc_health_check(slurm_msg_t *msg)
{
	int        rc = SLURM_SUCCESS;
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);

	if (!_slurm_authorized_user(req_uid)) {
		error("Security violation, health check RPC from uid %d",
		      req_uid);
		rc = ESLURM_USER_ID_MISSING;	 
	}

	 
	if (slurm_send_rc_msg(msg, rc) < 0) {
		error("Error responding to health check: %m");
		send_registration_msg(SLURM_SUCCESS, false);
	}

	if (rc == SLURM_SUCCESS)
		rc = run_script_health_check();

	 
	_enforce_job_mem_limit();
	 
	_file_bcast_cleanup();
	return rc;
}
