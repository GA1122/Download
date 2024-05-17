_forkexec_slurmstepd(uint16_t type, void *req,
		     slurm_addr_t *cli, slurm_addr_t *self,
		     const hostset_t step_hset, uint16_t protocol_version)
{
	pid_t pid;
	int to_stepd[2] = {-1, -1};
	int to_slurmd[2] = {-1, -1};

	if (pipe(to_stepd) < 0 || pipe(to_slurmd) < 0) {
		error("_forkexec_slurmstepd pipe failed: %m");
		return SLURM_FAILURE;
	}

	if (_add_starting_step(type, req)) {
		error("_forkexec_slurmstepd failed in _add_starting_step: %m");
		return SLURM_FAILURE;
	}

	if ((pid = fork()) < 0) {
		error("_forkexec_slurmstepd: fork: %m");
		close(to_stepd[0]);
		close(to_stepd[1]);
		close(to_slurmd[0]);
		close(to_slurmd[1]);
		_remove_starting_step(type, req);
		return SLURM_FAILURE;
	} else if (pid > 0) {
		int rc = SLURM_SUCCESS;
#if (SLURMSTEPD_MEMCHECK == 0)
		int i;
		time_t start_time = time(NULL);
#endif
		 
		if (close(to_stepd[0]) < 0)
			error("Unable to close read to_stepd in parent: %m");
		if (close(to_slurmd[1]) < 0)
			error("Unable to close write to_slurmd in parent: %m");

		if ((rc = _send_slurmstepd_init(to_stepd[1], type,
						req, cli, self,
						step_hset,
						protocol_version)) != 0) {
			error("Unable to init slurmstepd");
			goto done;
		}

		 
#if (SLURMSTEPD_MEMCHECK == 0)
		i = read(to_slurmd[0], &rc, sizeof(int));
		if (i < 0) {
			error("%s: Can not read return code from slurmstepd "
			      "got %d: %m", __func__, i);
			rc = SLURM_FAILURE;
		} else if (i != sizeof(int)) {
			error("%s: slurmstepd failed to send return code "
			      "got %d: %m", __func__, i);
			rc = SLURM_FAILURE;
		} else {
			int delta_time = time(NULL) - start_time;
			int cc;
			if (delta_time > 5) {
				info("Warning: slurmstepd startup took %d sec, "
				     "possible file system problem or full "
				     "memory", delta_time);
			}
			if (rc != SLURM_SUCCESS)
				error("slurmstepd return code %d", rc);

			cc = SLURM_SUCCESS;
			cc = write(to_stepd[1], &cc, sizeof(int));
			if (cc != sizeof(int)) {
				error("%s: failed to send ack to stepd %d: %m",
				      __func__, cc);
			}
		}
#endif
	done:
		if (_remove_starting_step(type, req))
			error("Error cleaning up starting_step list");

		 
		if (waitpid(pid, NULL, 0) < 0)
			error("Unable to reap slurmd child process");
		if (close(to_stepd[1]) < 0)
			error("close write to_stepd in parent: %m");
		if (close(to_slurmd[0]) < 0)
			error("close read to_slurmd in parent: %m");
		return rc;
	} else {
#if (SLURMSTEPD_MEMCHECK == 1)
		 
		char *const argv[3] = {"memcheck",
				       (char *)conf->stepd_loc, NULL};
#elif (SLURMSTEPD_MEMCHECK == 2)
		 
		uint32_t job_id = 0, step_id = 0;
		char log_file[256];
		char *const argv[13] = {"valgrind", "--tool=memcheck",
					"--error-limit=no",
					"--leak-check=summary",
					"--show-reachable=yes",
					"--max-stackframe=16777216",
					"--num-callers=20",
					"--child-silent-after-fork=yes",
					"--track-origins=yes",
					log_file, (char *)conf->stepd_loc,
					NULL};
		if (type == LAUNCH_BATCH_JOB) {
			job_id = ((batch_job_launch_msg_t *)req)->job_id;
			step_id = ((batch_job_launch_msg_t *)req)->step_id;
		} else if (type == LAUNCH_TASKS) {
			job_id = ((launch_tasks_request_msg_t *)req)->job_id;
			step_id = ((launch_tasks_request_msg_t *)req)->job_step_id;
		}
		snprintf(log_file, sizeof(log_file),
			 "--log-file=/tmp/slurmstepd_valgrind_%u.%u",
			 job_id, step_id);
#elif (SLURMSTEPD_MEMCHECK == 3)
		 
		uint32_t job_id = 0, step_id = 0;
		char log_file[256];
		char *const argv[10] = {"valgrind", "--tool=drd",
					"--error-limit=no",
					"--max-stackframe=16777216",
					"--num-callers=20",
					"--child-silent-after-fork=yes",
					log_file, (char *)conf->stepd_loc,
					NULL};
		if (type == LAUNCH_BATCH_JOB) {
			job_id = ((batch_job_launch_msg_t *)req)->job_id;
			step_id = ((batch_job_launch_msg_t *)req)->step_id;
		} else if (type == LAUNCH_TASKS) {
			job_id = ((launch_tasks_request_msg_t *)req)->job_id;
			step_id = ((launch_tasks_request_msg_t *)req)->job_step_id;
		}
		snprintf(log_file, sizeof(log_file),
			 "--log-file=/tmp/slurmstepd_valgrind_%u.%u",
			 job_id, step_id);
#elif (SLURMSTEPD_MEMCHECK == 4)
		 
		uint32_t job_id = 0, step_id = 0;
		char log_file[256];
		char *const argv[10] = {"valgrind", "--tool=helgrind",
					"--error-limit=no",
					"--max-stackframe=16777216",
					"--num-callers=20",
					"--child-silent-after-fork=yes",
					log_file, (char *)conf->stepd_loc,
					NULL};
		if (type == LAUNCH_BATCH_JOB) {
			job_id = ((batch_job_launch_msg_t *)req)->job_id;
			step_id = ((batch_job_launch_msg_t *)req)->step_id;
		} else if (type == LAUNCH_TASKS) {
			job_id = ((launch_tasks_request_msg_t *)req)->job_id;
			step_id = ((launch_tasks_request_msg_t *)req)->job_step_id;
		}
		snprintf(log_file, sizeof(log_file),
			 "--log-file=/tmp/slurmstepd_valgrind_%u.%u",
			 job_id, step_id);
#else
		 
		char *const argv[2] = { (char *)conf->stepd_loc, NULL};
#endif
		int i;
		int failed = 0;
		 
		setenv("SLURM_CONF", conf->conffile, 1);

		 
		if (setsid() < 0) {
			error("_forkexec_slurmstepd: setsid: %m");
			failed = 1;
		}
		if ((pid = fork()) < 0) {
			error("_forkexec_slurmstepd: "
			      "Unable to fork grandchild: %m");
			failed = 2;
		} else if (pid > 0) {  
			exit(0);
		}

		 
		for (i=3; i<256; i++) {
			(void) fcntl(i, F_SETFD, FD_CLOEXEC);
		}

		 
		if ((to_stepd[0] != conf->lfd)
		    && (to_slurmd[1] != conf->lfd))
			slurm_shutdown_msg_engine(conf->lfd);

		if (close(to_stepd[1]) < 0)
			error("close write to_stepd in grandchild: %m");
		if (close(to_slurmd[0]) < 0)
			error("close read to_slurmd in parent: %m");

		(void) close(STDIN_FILENO);  
		if (dup2(to_stepd[0], STDIN_FILENO) == -1) {
			error("dup2 over STDIN_FILENO: %m");
			exit(1);
		}
		fd_set_close_on_exec(to_stepd[0]);
		(void) close(STDOUT_FILENO);  
		if (dup2(to_slurmd[1], STDOUT_FILENO) == -1) {
			error("dup2 over STDOUT_FILENO: %m");
			exit(1);
		}
		fd_set_close_on_exec(to_slurmd[1]);
		(void) close(STDERR_FILENO);  
		if (dup2(devnull, STDERR_FILENO) == -1) {
			error("dup2 /dev/null to STDERR_FILENO: %m");
			exit(1);
		}
		fd_set_noclose_on_exec(STDERR_FILENO);
		log_fini();
		if (!failed) {
			if (conf->chos_loc && !access(conf->chos_loc, X_OK))
				execvp(conf->chos_loc, argv);
			else
				execvp(argv[0], argv);
			error("exec of slurmstepd failed: %m");
		}
		exit(2);
	}
}
