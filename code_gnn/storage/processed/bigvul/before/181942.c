 static int attach_child_main(void* data)
  {
  	struct attach_clone_payload* payload = (struct attach_clone_payload*)data;
  	int ipc_socket = payload->ipc_socket;
	int procfd = payload->procfd;
  	lxc_attach_options_t* options = payload->options;
  	struct lxc_proc_context_info* init_ctx = payload->init_ctx;
  #if HAVE_SYS_PERSONALITY_H
 	long new_personality;
 #endif
 	int ret;
 	int status;
  	int expected;
  	long flags;
  	int fd;
// 	int lsm_labelfd;
  	uid_t new_uid;
  	gid_t new_gid;
  
 	 
 	expected = 0;
  	status = -1;
  	ret = lxc_read_nointr_expect(ipc_socket, &status, sizeof(status), &expected);
  	if (ret <= 0) {
		ERROR("error using IPC to receive notification from initial process (0)");
// 		ERROR("Error using IPC to receive notification from initial process (0): %s.", strerror(errno));
  		shutdown(ipc_socket, SHUT_RDWR);
  		rexit(-1);
  	}
 
 	 
 	if (!(options->namespaces & CLONE_NEWNS) && (options->attach_flags & LXC_ATTACH_REMOUNT_PROC_SYS)) {
 		ret = lxc_attach_remount_sys_proc();
 		if (ret < 0) {
 			shutdown(ipc_socket, SHUT_RDWR);
 			rexit(-1);
 		}
 	}
 
 	 
 #if HAVE_SYS_PERSONALITY_H
 	if (options->personality < 0)
 		new_personality = init_ctx->personality;
 	else
 		new_personality = options->personality;
 
 	if (options->attach_flags & LXC_ATTACH_SET_PERSONALITY) {
 		ret = personality(new_personality);
 		if (ret < 0) {
 			SYSERROR("could not ensure correct architecture");
 			shutdown(ipc_socket, SHUT_RDWR);
 			rexit(-1);
 		}
 	}
 #endif
 
 	if (options->attach_flags & LXC_ATTACH_DROP_CAPABILITIES) {
 		ret = lxc_attach_drop_privs(init_ctx);
 		if (ret < 0) {
 			ERROR("could not drop privileges");
 			shutdown(ipc_socket, SHUT_RDWR);
 			rexit(-1);
 		}
 	}
 
 	 
 	ret = lxc_attach_set_environment(options->env_policy, options->extra_env_vars, options->extra_keep_env);
 	if (ret < 0) {
 		ERROR("could not set initial environment for attached process");
 		shutdown(ipc_socket, SHUT_RDWR);
 		rexit(-1);
 	}
 
 	 
 	new_uid = 0;
 	new_gid = 0;
 	 
 	if (options->namespaces & CLONE_NEWUSER)
 		lxc_attach_get_init_uidgid(&new_uid, &new_gid);
 
 	if (options->uid != (uid_t)-1)
 		new_uid = options->uid;
 	if (options->gid != (gid_t)-1)
 		new_gid = options->gid;
 
 	 
 	if (options->stdin_fd && isatty(options->stdin_fd)) {
 		if (setsid() < 0) {
 			SYSERROR("unable to setsid");
 			shutdown(ipc_socket, SHUT_RDWR);
 			rexit(-1);
 		}
 
 		if (ioctl(options->stdin_fd, TIOCSCTTY, (char *)NULL) < 0) {
 			SYSERROR("unable to TIOCSTTY");
 			shutdown(ipc_socket, SHUT_RDWR);
 			rexit(-1);
 		}
 	}
 
 	 
 	if ((new_gid != 0 || options->namespaces & CLONE_NEWUSER)) {
 		if (setgid(new_gid) || setgroups(0, NULL)) {
 			SYSERROR("switching to container gid");
 			shutdown(ipc_socket, SHUT_RDWR);
 			rexit(-1);
 		}
 	}
 	if ((new_uid != 0 || options->namespaces & CLONE_NEWUSER) && setuid(new_uid)) {
 		SYSERROR("switching to container uid");
 		shutdown(ipc_socket, SHUT_RDWR);
 		rexit(-1);
 	}
 
 	 
  	status = 1;
  	ret = lxc_write_nointr(ipc_socket, &status, sizeof(status));
  	if (ret != sizeof(status)) {
		ERROR("error using IPC to notify initial process for initialization (1)");
// 		ERROR("Error using IPC to notify initial process for initialization (1): %s.", strerror(errno));
  		shutdown(ipc_socket, SHUT_RDWR);
  		rexit(-1);
  	}
 
 	 
 	expected = 2;
  	status = -1;
  	ret = lxc_read_nointr_expect(ipc_socket, &status, sizeof(status), &expected);
  	if (ret <= 0) {
		ERROR("error using IPC to receive final notification from initial process (2)");
// 		ERROR("Error using IPC to receive message from initial process "
// 		      "that it is done pre-initializing (2): %s",
// 		      strerror(errno));
  		shutdown(ipc_socket, SHUT_RDWR);
  		rexit(-1);
  	}
  
	shutdown(ipc_socket, SHUT_RDWR);
	close(ipc_socket);
  	if ((init_ctx->container && init_ctx->container->lxc_conf &&
  	     init_ctx->container->lxc_conf->no_new_privs) ||
  	    (options->attach_flags & LXC_ATTACH_NO_NEW_PRIVS)) {
  		if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0) {
  			SYSERROR("PR_SET_NO_NEW_PRIVS could not be set. "
  				 "Process can use execve() gainable "
  				 "privileges.");
// 			shutdown(ipc_socket, SHUT_RDWR);
  			rexit(-1);
  		}
  		INFO("PR_SET_NO_NEW_PRIVS is set. Process cannot use execve() "
  		     "gainable privileges.");
  	}
  
 	 
// 	status = 3;
// 	ret = lxc_write_nointr(ipc_socket, &status, sizeof(status));
// 	if (ret <= 0) {
// 		ERROR("Error using IPC to tell parent to set up LSM labels (3): %s.", strerror(errno));
// 		shutdown(ipc_socket, SHUT_RDWR);
// 		rexit(-1);
// 	}
// 
  	if ((options->namespaces & CLONE_NEWNS) && (options->attach_flags & LXC_ATTACH_LSM) && init_ctx->lsm_label) {
  		int on_exec;
// 		 
// 		ret = lxc_abstract_unix_recv_fd(ipc_socket, &lsm_labelfd, NULL, 0);
// 		if (ret <= 0) {
// 			ERROR("Error using IPC for parent to tell us LSM label fd (4): %s.", strerror(errno));
// 			shutdown(ipc_socket, SHUT_RDWR);
// 			rexit(-1);
// 		}
  
// 		 
  		on_exec = options->attach_flags & LXC_ATTACH_LSM_EXEC ? 1 : 0;
		if (lsm_set_label_at(procfd, on_exec, init_ctx->lsm_label) < 0) {
// 		if (lsm_set_label_at(lsm_labelfd, on_exec, init_ctx->lsm_label) < 0) {
// 			SYSERROR("Failed to set LSM label.");
// 			shutdown(ipc_socket, SHUT_RDWR);
// 			close(lsm_labelfd);
  			rexit(-1);
  		}
// 		close(lsm_labelfd);
  	}
// 
  	if (init_ctx->container && init_ctx->container->lxc_conf &&
  	    init_ctx->container->lxc_conf->seccomp &&
  	    (lxc_seccomp_load(init_ctx->container->lxc_conf) != 0)) {
  		ERROR("Loading seccomp policy");
// 		shutdown(ipc_socket, SHUT_RDWR);
  		rexit(-1);
  	}
// 
// 	shutdown(ipc_socket, SHUT_RDWR);
// 	close(ipc_socket);
  	lxc_proc_put_context_info(init_ctx);
  
  	 
 
 	 
 	if (options->stdin_fd >= 0 && options->stdin_fd != 0)
 		dup2(options->stdin_fd, 0);
 	if (options->stdout_fd >= 0 && options->stdout_fd != 1)
 		dup2(options->stdout_fd, 1);
 	if (options->stderr_fd >= 0 && options->stderr_fd != 2)
 		dup2(options->stderr_fd, 2);
 
 	 
 	if (options->stdin_fd > 2)
 		close(options->stdin_fd);
 	if (options->stdout_fd > 2)
 		close(options->stdout_fd);
 	if (options->stderr_fd > 2)
 		close(options->stderr_fd);
 
 	 
 	for (fd = 0; fd <= 2; fd++) {
 		flags = fcntl(fd, F_GETFL);
 		if (flags < 0)
 			continue;
 		if (flags & FD_CLOEXEC)
 			if (fcntl(fd, F_SETFL, flags & ~FD_CLOEXEC) < 0)
  				SYSERROR("Unable to clear CLOEXEC from fd");
  	}
  
	 
	close(procfd);
  	 
  	rexit(payload->exec_function(payload->exec_payload));
  }