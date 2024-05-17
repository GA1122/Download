static int _file_bcast_register_file(slurm_msg_t *msg,
				     file_bcast_info_t *key)
{
	file_bcast_msg_t *req = msg->data;
	int fd, flags, rc;
	int pipe[2];
	gids_t *gids;
	pid_t child;
	file_bcast_info_t *file_info;

	if (!(gids = _gids_cache_lookup(req->user_name, key->gid))) {
		error("sbcast: gids_cache_lookup for %s failed", req->user_name);
		return SLURM_ERROR;
	}

	if ((rc = container_g_create(key->job_id))) {
		error("sbcast: container_g_create(%u): %m", key->job_id);
		_dealloc_gids(gids);
		return rc;
	}

	 

	if (socketpair(AF_UNIX, SOCK_DGRAM, 0, pipe) != 0) {
		error("%s: Failed to open pipe: %m", __func__);
		_dealloc_gids(gids);
		return SLURM_ERROR;
	}

	child = fork();
	if (child == -1) {
		error("sbcast: fork failure");
		_dealloc_gids(gids);
		close(pipe[0]);
		close(pipe[1]);
		return errno;
	} else if (child > 0) {
		 
		close(pipe[0]);
		waitpid(child, &rc, 0);
		_dealloc_gids(gids);
		if (rc) {
			close(pipe[1]);
			return WEXITSTATUS(rc);
		}

		fd = _receive_fd(pipe[1]);
		close(pipe[1]);

		file_info = xmalloc(sizeof(file_bcast_info_t));
		file_info->fd = fd;
		file_info->fname = xstrdup(req->fname);
		file_info->uid = key->uid;
		file_info->gid = key->gid;
		file_info->job_id = key->job_id;
		file_info->start_time = time(NULL);

		_fb_wrlock();
		list_append(file_bcast_list, file_info);
		_fb_wrunlock();

		return SLURM_SUCCESS;
	}

	 

	close(pipe[1]);

	 
	if (container_g_add_pid(key->job_id, getpid(), key->uid)) {
		error("container_g_add_pid(%u): %m", key->job_id);
		exit(SLURM_ERROR);
	}

	 

	 

	if (setgroups(gids->ngids, gids->gids) < 0) {
		error("sbcast: uid: %u setgroups failed: %m", key->uid);
		exit(errno);
	}
	_dealloc_gids(gids);

	if (setgid(key->gid) < 0) {
		error("sbcast: uid:%u setgid(%u): %m", key->uid, key->gid);
		exit(errno);
	}
	if (setuid(key->uid) < 0) {
		error("sbcast: getuid(%u): %m", key->uid);
		exit(errno);
	}

	flags = O_WRONLY | O_CREAT;
	if (req->force)
		flags |= O_TRUNC;
	else
		flags |= O_EXCL;

	fd = open(key->fname, flags, 0700);
	if (fd == -1) {
		error("sbcast: uid:%u can't open `%s`: %m",
		      key->uid, key->fname);
		exit(errno);
	}
	_send_back_fd(pipe[0], fd);
	close(fd);
	exit(SLURM_SUCCESS);
}
