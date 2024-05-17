 run_cmd(int fd, ...)
// run_cmd(int fd, const char *cmd, const char *args_extra)
  {
  	pid_t pid;
  	sigset_t sigm, sigm_old;
 
 	 
 	sigemptyset(&sigm);
 	sigaddset(&sigm, SIGTERM);
 	sigprocmask(SIG_BLOCK, &sigm, &sigm_old);
 
 	pid = fork();
 	if ( pid < 0 ) {
 		sigprocmask(SIG_SETMASK, &sigm_old, NULL);
 		fd_printf(STO, "*** cannot fork: %s ***\r\n", strerror(errno));
 		return -1;
 	} else if ( pid ) {
 		 
 		int status, r;
 
 		 
 		sigprocmask(SIG_SETMASK, &sigm_old, NULL);
 		 
 		do {
 			r = waitpid(pid, &status, 0);
 		} while ( r < 0 && errno == EINTR );
 		 
 		term_apply(STI);
 		 
 		if ( WIFEXITED(status) ) { 
 			fd_printf(STO, "\r\n*** exit status: %d ***\r\n", 
 					  WEXITSTATUS(status));
 			return WEXITSTATUS(status);
 		} else if ( WIFSIGNALED(status) ) {
 			fd_printf(STO, "\r\n*** killed by signal: %d ***\r\n", 
 					  WTERMSIG(status));
 			return -1;
 		} else {
 			fd_printf(STO, "\r\n*** abnormal termination: 0x%x ***\r\n", r);
 			return -1;
 		}
  	} else {
  		 
  		long fl;
		char cmd[512];
// 		int argc;
// 		char *argv[RUNCMD_ARGS_MAX + 1];
// 		int r;
// 			
  		 
  		term_remove(STI);
  		 
 		term_erase(fd);
 		 
 		fl = fcntl(fd, F_GETFL); 
 		fl &= ~O_NONBLOCK;
 		fcntl(fd, F_SETFL, fl);
 		 
 		close(STI);
  		close(STO);
  		dup2(fd, STI);
  		dup2(fd, STO);
		{
			 
			char *c, *ce;
			const char *s;
			int n;
			va_list vls;
			strcpy(cmd, EXEC);
			c = &cmd[sizeof(EXEC)- 1];
			ce = cmd + sizeof(cmd) - 1;
			va_start(vls, fd);
			while ( (s = va_arg(vls, const char *)) ) {
				n = strlen(s);
				if ( c + n + 1 >= ce ) break;
				memcpy(c, s, n); c += n;
				*c++ = ' ';
			}
			va_end(vls);
			*c = '\0';
// 		
// 		 
// 		argc = 0;
// 		r = split_quoted(cmd, &argc, argv, RUNCMD_ARGS_MAX);
// 		if ( r < 0 ) {
// 			fd_printf(STDERR_FILENO, "Cannot parse command\n");
// 			exit(RUNCMD_EXEC_FAIL);
// 		}
// 		r = split_quoted(args_extra, &argc, argv, RUNCMD_ARGS_MAX);
// 		if ( r < 0 ) {
// 			fd_printf(STDERR_FILENO, "Cannot parse extra args\n");
// 			exit(RUNCMD_EXEC_FAIL);
  		}
// 		if ( argc < 1 ) {
// 			fd_printf(STDERR_FILENO, "No command given\n");
// 			exit(RUNCMD_EXEC_FAIL);
// 		}	
// 		argv[argc] = NULL;
// 			
  		 
		fd_printf(STDERR_FILENO, "%s\n", &cmd[sizeof(EXEC) - 1]);
// 		fd_printf(STDERR_FILENO, "$ %s %s\n", cmd, args_extra);
  		establish_child_signal_handlers();
  		sigprocmask(SIG_SETMASK, &sigm_old, NULL);
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		exit(42);
// 		execvp(argv[0], argv);
// 
// 		fd_printf(STDERR_FILENO, "exec: %s\n", strerror(errno));
// 		exit(RUNCMD_EXEC_FAIL);
  	}
  }