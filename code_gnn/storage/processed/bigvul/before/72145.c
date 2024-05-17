_send_slurmstepd_init(int fd, int type, void *req,
		      slurm_addr_t *cli, slurm_addr_t *self,
		      hostset_t step_hset, uint16_t protocol_version)
{
	int len = 0;
	Buf buffer = NULL;
	slurm_msg_t msg;
	uid_t uid = (uid_t)-1;
	gid_t gid = (uid_t)-1;
	gids_t *gids = NULL;

	int rank, proto;
	int parent_rank, children, depth, max_depth;
	char *parent_alias = NULL;
	char *user_name = NULL;
	slurm_addr_t parent_addr = {0};
	char pwd_buffer[PW_BUF_SIZE];
	struct passwd pwd, *pwd_result;

	slurm_msg_t_init(&msg);
	 
	safe_write(fd, &type, sizeof(int));

	 
	if (step_hset == NULL) {
		bool send_error = false;
		if (type == LAUNCH_TASKS) {
			launch_tasks_request_msg_t *launch_req;
			launch_req = (launch_tasks_request_msg_t *) req;
			if (launch_req->job_step_id != SLURM_EXTERN_CONT)
				send_error = true;
		}
		if (send_error) {
			info("task rank unavailable due to invalid job "
			     "credential, step completion RPC impossible");
		}
		rank = -1;
		parent_rank = -1;
		children = 0;
		depth = 0;
		max_depth = 0;
	} else if ((type == LAUNCH_TASKS) &&
		   (((launch_tasks_request_msg_t *)req)->alias_list)) {
		 
		rank = 0;
		parent_rank = -1;
		children = 0;
		depth = 0;
		max_depth = 0;
	} else {
#ifndef HAVE_FRONT_END
		int count;
		count = hostset_count(step_hset);
		rank = hostset_find(step_hset, conf->node_name);
		reverse_tree_info(rank, count, REVERSE_TREE_WIDTH,
				  &parent_rank, &children,
				  &depth, &max_depth);
		if (rank > 0) {  
			int rc;
			 
			parent_alias = hostset_nth(step_hset, parent_rank);
			rc = slurm_conf_get_addr(parent_alias, &parent_addr);
			if (rc != SLURM_SUCCESS) {
				error("Failed looking up address for "
				      "NodeName %s", parent_alias);
				 
			}
		}
#else
		 
		rank = 0;
		parent_rank = -1;
		children = 0;
		depth = 0;
		max_depth = 0;
#endif
	}
	debug3("slurmstepd rank %d (%s), parent rank %d (%s), "
	       "children %d, depth %d, max_depth %d",
	       rank, conf->node_name,
	       parent_rank, parent_alias ? parent_alias : "NONE",
	       children, depth, max_depth);
	if (parent_alias)
		free(parent_alias);

	 
	safe_write(fd, &rank, sizeof(int));
	safe_write(fd, &parent_rank, sizeof(int));
	safe_write(fd, &children, sizeof(int));
	safe_write(fd, &depth, sizeof(int));
	safe_write(fd, &max_depth, sizeof(int));
	safe_write(fd, &parent_addr, sizeof(slurm_addr_t));

	 
	if (_send_slurmd_conf_lite(fd, conf) < 0)
		goto rwfail;

	 
	buffer = init_buf(0);
	slurm_pack_slurm_addr(cli, buffer);
	len = get_buf_offset(buffer);
	safe_write(fd, &len, sizeof(int));
	safe_write(fd, get_buf_data(buffer), len);
	free_buf(buffer);
	buffer = NULL;

	 
	if (self) {
		buffer = init_buf(0);
		slurm_pack_slurm_addr(self, buffer);
		len = get_buf_offset(buffer);
		safe_write(fd, &len, sizeof(int));
		safe_write(fd, get_buf_data(buffer), len);
		free_buf(buffer);
		buffer = NULL;

	} else {
		len = 0;
		safe_write(fd, &len, sizeof(int));
	}

	 
	gres_plugin_send_stepd(fd);

	 
	cpu_freq_send_info(fd);

	 
	switch(type) {
	case LAUNCH_BATCH_JOB:
		gid = (uid_t)((batch_job_launch_msg_t *)req)->gid;
		uid = (uid_t)((batch_job_launch_msg_t *)req)->uid;
		user_name = ((batch_job_launch_msg_t *)req)->user_name;
		msg.msg_type = REQUEST_BATCH_JOB_LAUNCH;
		break;
	case LAUNCH_TASKS:
		 
		gid = (uid_t)((launch_tasks_request_msg_t *)req)->gid;
		uid = (uid_t)((launch_tasks_request_msg_t *)req)->uid;
		user_name = ((launch_tasks_request_msg_t *)req)->user_name;
		msg.msg_type = REQUEST_LAUNCH_TASKS;
		break;
	default:
		error("Was sent a task I didn't understand");
		break;
	}
	buffer = init_buf(0);
	msg.data = req;

	if (protocol_version == (uint16_t)NO_VAL)
		proto = SLURM_PROTOCOL_VERSION;
	else
		proto = protocol_version;

	msg.protocol_version = (uint16_t)proto;
	pack_msg(&msg, buffer);
	len = get_buf_offset(buffer);

	safe_write(fd, &proto, sizeof(int));

	safe_write(fd, &len, sizeof(int));
	safe_write(fd, get_buf_data(buffer), len);
	free_buf(buffer);
	buffer = NULL;

#ifdef HAVE_NATIVE_CRAY
	 
	if (!user_name) {
#endif
		 
		debug3("_send_slurmstepd_init: call to getpwuid_r");
		if (slurm_getpwuid_r(uid, &pwd, pwd_buffer, PW_BUF_SIZE,
				     &pwd_result) || (pwd_result == NULL)) {
			error("%s: getpwuid_r: %m", __func__);
			len = 0;
			safe_write(fd, &len, sizeof(int));
			errno = ESLURMD_UID_NOT_FOUND;
			return errno;
		}
		debug3("%s: return from getpwuid_r", __func__);
		if (gid != pwd_result->pw_gid) {
			debug("%s: Changing gid from %d to %d",
			      __func__, gid, pwd_result->pw_gid);
		}
		gid = pwd_result->pw_gid;
		if (!user_name)
			user_name = pwd_result->pw_name;
#ifdef HAVE_NATIVE_CRAY
	}
#endif
	if (!user_name) {
		 
		error("%s: No user name for %d: %m", __func__, uid);
		len = 0;
		safe_write(fd, &len, sizeof(int));
		errno = ESLURMD_UID_NOT_FOUND;
		return errno;
	}

	if ((gids = _gids_cache_lookup(user_name, gid))) {
		int i;
		uint32_t tmp32;
		safe_write(fd, &gids->ngids, sizeof(int));
		for (i = 0; i < gids->ngids; i++) {
			tmp32 = (uint32_t)gids->gids[i];
			safe_write(fd, &tmp32, sizeof(uint32_t));
		}
		_dealloc_gids(gids);
	} else {
		len = 0;
		safe_write(fd, &len, sizeof(int));
	}
	return 0;

rwfail:
	if (buffer)
		free_buf(buffer);
	error("_send_slurmstepd_init failed");
	return errno;
}
