int fpm_children_make(struct fpm_worker_pool_s *wp, int in_event_loop, int nb_to_spawn, int is_debug)  
{
	pid_t pid;
	struct fpm_child_s *child;
	int max;
	static int warned = 0;

	if (wp->config->pm == PM_STYLE_DYNAMIC) {
		if (!in_event_loop) {  
			max = wp->config->pm_start_servers;
		} else {
			max = wp->running_children + nb_to_spawn;
		}
	} else if (wp->config->pm == PM_STYLE_ONDEMAND) {
		if (!in_event_loop) {  
			max = 0;  
		} else {
			max = wp->running_children + nb_to_spawn;
		}
	} else {  
		max = wp->config->pm_max_children;
	}

	 
	while (fpm_pctl_can_spawn_children() && wp->running_children < max && (fpm_global_config.process_max < 1 || fpm_globals.running_children < fpm_global_config.process_max)) {

		warned = 0;
		child = fpm_resources_prepare(wp);

		if (!child) {
			return 2;
		}

		pid = fork();

		switch (pid) {

			case 0 :
				fpm_child_resources_use(child);
				fpm_globals.is_child = 1;
				fpm_child_init(wp);
				return 0;

			case -1 :
				zlog(ZLOG_SYSERROR, "fork() failed");

				fpm_resources_discard(child);
				return 2;

			default :
				child->pid = pid;
				fpm_clock_get(&child->started);
				fpm_parent_resources_use(child);

				zlog(is_debug ? ZLOG_DEBUG : ZLOG_NOTICE, "[pool %s] child %d started", wp->config->name, (int) pid);
		}

	}

	if (!warned && fpm_global_config.process_max > 0 && fpm_globals.running_children >= fpm_global_config.process_max) {
               if (wp->running_children < max) {
                       warned = 1;
                       zlog(ZLOG_WARNING, "The maximum number of processes has been reached. Please review your configuration and consider raising 'process.max'");
               }
	}

	return 1;  
}
 