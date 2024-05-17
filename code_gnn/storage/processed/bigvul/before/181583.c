 main (int    argc,
       char **argv)
 {
   mode_t old_umask;
   cleanup_free char *base_path = NULL;
   int clone_flags;
   char *old_cwd = NULL;
   pid_t pid;
   int event_fd = -1;
   int child_wait_fd = -1;
   const char *new_cwd;
   uid_t ns_uid;
   gid_t ns_gid;
   struct stat sbuf;
   uint64_t val;
   int res UNUSED;
 
   real_uid = getuid ();
   real_gid = getgid ();
 
    
   acquire_privs ();
 
    
   if (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0)
     die_with_error ("prctl(PR_SET_NO_NEW_CAPS) failed");
 
    
 
   read_overflowids ();
 
   argv0 = argv[0];
 
   if (isatty (1))
     host_tty_dev = ttyname (1);
 
   argv++;
   argc--;
 
   if (argc == 0)
     usage (EXIT_FAILURE, stderr);
 
   parse_args (&argc, &argv);
 
    
   if (!is_privileged && getuid () != 0)
     opt_unshare_user = TRUE;
 
   if (opt_unshare_user_try &&
       stat ("/proc/self/ns/user", &sbuf) == 0)
     {
       bool disabled = FALSE;
 
        
       if (stat ("/sys/module/user_namespace/parameters/enable", &sbuf) == 0)
         {
           cleanup_free char *enable = NULL;
           enable = load_file_at (AT_FDCWD, "/sys/module/user_namespace/parameters/enable");
           if (enable != NULL && enable[0] == 'N')
             disabled = TRUE;
         }
 
        
 
       if (!disabled)
         opt_unshare_user = TRUE;
     }
 
   if (argc == 0)
     usage (EXIT_FAILURE, stderr);
 
   __debug__ (("Creating root mount point\n"));
 
   if (opt_sandbox_uid == -1)
     opt_sandbox_uid = real_uid;
   if (opt_sandbox_gid == -1)
     opt_sandbox_gid = real_gid;
 
   if (!opt_unshare_user && opt_sandbox_uid != real_uid)
     die ("Specifying --uid requires --unshare-user");
 
   if (!opt_unshare_user && opt_sandbox_gid != real_gid)
     die ("Specifying --gid requires --unshare-user");
 
   if (!opt_unshare_uts && opt_sandbox_hostname != NULL)
     die ("Specifying --hostname requires --unshare-uts");
 
    
   proc_fd = open ("/proc", O_RDONLY | O_PATH);
   if (proc_fd == -1)
     die_with_error ("Can't open /proc");
 
    
   base_path = xasprintf ("/run/user/%d/.bubblewrap", real_uid);
   if (mkdir (base_path, 0755) && errno != EEXIST)
     {
       free (base_path);
       base_path = xasprintf ("/tmp/.bubblewrap-%d", real_uid);
       if (mkdir (base_path, 0755) && errno != EEXIST)
         die_with_error ("Creating root mountpoint failed");
     }
 
   __debug__ (("creating new namespace\n"));
 
   if (opt_unshare_pid)
     {
       event_fd = eventfd (0, EFD_CLOEXEC | EFD_NONBLOCK);
       if (event_fd == -1)
         die_with_error ("eventfd()");
     }
 
    
   block_sigchild ();
 
   clone_flags = SIGCHLD | CLONE_NEWNS;
   if (opt_unshare_user)
     clone_flags |= CLONE_NEWUSER;
   if (opt_unshare_pid)
     clone_flags |= CLONE_NEWPID;
   if (opt_unshare_net)
     clone_flags |= CLONE_NEWNET;
   if (opt_unshare_ipc)
     clone_flags |= CLONE_NEWIPC;
   if (opt_unshare_uts)
     clone_flags |= CLONE_NEWUTS;
   if (opt_unshare_cgroup)
     {
       if (stat ("/proc/self/ns/cgroup", &sbuf))
         {
           if (errno == ENOENT)
             die ("Cannot create new cgroup namespace because the kernel does not support it");
           else
             die_with_error ("stat on /proc/self/ns/cgroup failed");
         }
       clone_flags |= CLONE_NEWCGROUP;
     }
   if (opt_unshare_cgroup_try)
     if (!stat ("/proc/self/ns/cgroup", &sbuf))
       clone_flags |= CLONE_NEWCGROUP;
 
   child_wait_fd = eventfd (0, EFD_CLOEXEC);
   if (child_wait_fd == -1)
     die_with_error ("eventfd()");
 
   pid = raw_clone (clone_flags, NULL);
   if (pid == -1)
     {
       if (opt_unshare_user)
         {
           if (errno == EINVAL)
             die ("Creating new namespace failed, likely because the kernel does not support user namespaces.  bwrap must be installed setuid on such systems.");
           else if (errno == EPERM && !is_privileged)
             die ("No permissions to creating new namespace, likely because the kernel does not allow non-privileged user namespaces. On e.g. debian this can be enabled with 'sysctl kernel.unprivileged_userns_clone=1'.");
         }
 
       die_with_error ("Creating new namespace failed");
     }
 
   ns_uid = opt_sandbox_uid;
   ns_gid = opt_sandbox_gid;
 
   if (pid != 0)
     {
        
 
       if (is_privileged && opt_unshare_user)
         {
            
           write_uid_gid_map (ns_uid, real_uid,
                              ns_gid, real_gid,
                              pid, TRUE, opt_needs_devpts);
         }
 
        
 
        
       drop_privs ();
 
        
       val = 1;
       res = write (child_wait_fd, &val, 8);
        
       close (child_wait_fd);
 
       if (opt_info_fd != -1)
         {
           cleanup_free char *output = xasprintf ("{\n    \"child-pid\": %i\n}\n", pid);
           size_t len = strlen (output);
           if (write (opt_info_fd, output, len) != len)
             die_with_error ("Write to info_fd");
           close (opt_info_fd);
         }
 
       monitor_child (event_fd);
       exit (0);  
     }
 
    
 
   if (opt_info_fd != -1)
     close (opt_info_fd);
 
    
   res = read (child_wait_fd, &val, 8);
   close (child_wait_fd);
 
    
   switch_to_user_with_privs ();
 
   if (opt_unshare_net && loopback_setup () != 0)
     die ("Can't create loopback device");
 
   ns_uid = opt_sandbox_uid;
   ns_gid = opt_sandbox_gid;
   if (!is_privileged && opt_unshare_user)
     {
        
 
       if (opt_needs_devpts)
         {
            
           ns_uid = 0;
           ns_gid = 0;
         }
 
       write_uid_gid_map (ns_uid, real_uid,
                          ns_gid, real_gid,
                          -1, TRUE, FALSE);
     }
 
   old_umask = umask (0);
 
    
   resolve_symlinks_in_ops ();
 
    
   if (mount (NULL, "/", NULL, MS_SLAVE | MS_REC, NULL) < 0)
     die_with_error ("Failed to make / slave");
 
    
   if (mount ("", base_path, "tmpfs", MS_NODEV | MS_NOSUID, NULL) != 0)
     die_with_error ("Failed to mount tmpfs");
 
   old_cwd = get_current_dir_name ();
 
    
   if (chdir (base_path) != 0)
     die_with_error ("chdir base_path");
 
    
 
   if (mkdir ("newroot", 0755))
     die_with_error ("Creating newroot failed");
 
   if (mkdir ("oldroot", 0755))
     die_with_error ("Creating oldroot failed");
 
   if (pivot_root (base_path, "oldroot"))
     die_with_error ("pivot_root");
 
   if (chdir ("/") != 0)
     die_with_error ("chdir / (base path)");
 
   if (is_privileged)
     {
       pid_t child;
       int privsep_sockets[2];
 
       if (socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, privsep_sockets) != 0)
         die_with_error ("Can't create privsep socket");
 
       child = fork ();
       if (child == -1)
         die_with_error ("Can't fork unprivileged helper");
 
       if (child == 0)
         {
            
           drop_privs ();
           close (privsep_sockets[0]);
           setup_newroot (opt_unshare_pid, privsep_sockets[1]);
           exit (0);
         }
       else
         {
           int status;
           uint32_t buffer[2048];   
           uint32_t op, flags;
           const char *arg1, *arg2;
           cleanup_fd int unpriv_socket = -1;
 
           unpriv_socket = privsep_sockets[0];
           close (privsep_sockets[1]);
 
           do
             {
               op = read_priv_sec_op (unpriv_socket, buffer, sizeof (buffer),
                                      &flags, &arg1, &arg2);
               privileged_op (-1, op, flags, arg1, arg2);
               if (write (unpriv_socket, buffer, 1) != 1)
                 die ("Can't write to op_socket");
             }
           while (op != PRIV_SEP_OP_DONE);
 
           waitpid (child, &status, 0);
            
         }
     }
   else
     {
       setup_newroot (opt_unshare_pid, -1);
     }
 
    
   if (mount ("oldroot", "oldroot", NULL, MS_REC | MS_PRIVATE, NULL) != 0)
     die_with_error ("Failed to make old root rprivate");
 
   if (umount2 ("oldroot", MNT_DETACH))
     die_with_error ("unmount old root");
 
   if (opt_unshare_user &&
       (ns_uid != opt_sandbox_uid || ns_gid != opt_sandbox_gid))
     {
        
 
       if (unshare (CLONE_NEWUSER))
         die_with_error ("unshare user ns");
 
       write_uid_gid_map (opt_sandbox_uid, ns_uid,
                          opt_sandbox_gid, ns_gid,
                          -1, FALSE, FALSE);
     }
 
    
   if (chdir ("/newroot") != 0)
     die_with_error ("chdir newroot");
   if (chroot ("/newroot") != 0)
     die_with_error ("chroot /newroot");
   if (chdir ("/") != 0)
     die_with_error ("chdir /");
 
    
   drop_privs ();
 
   if (opt_block_fd != -1)
     {
       char b[1];
       read (opt_block_fd, b, 1);
       close (opt_block_fd);
     }
 
   if (opt_seccomp_fd != -1)
     {
       cleanup_free char *seccomp_data = NULL;
       size_t seccomp_len;
       struct sock_fprog prog;
 
       seccomp_data = load_file_data (opt_seccomp_fd, &seccomp_len);
       if (seccomp_data == NULL)
         die_with_error ("Can't read seccomp data");
 
       if (seccomp_len % 8 != 0)
         die ("Invalid seccomp data, must be multiple of 8");
 
       prog.len = seccomp_len / 8;
       prog.filter = (struct sock_filter *) seccomp_data;
 
       close (opt_seccomp_fd);
 
       if (prctl (PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog) != 0)
         die_with_error ("prctl(PR_SET_SECCOMP)");
     }
 
   umask (old_umask);
 
   new_cwd = "/";
   if (opt_chdir_path)
     {
       if (chdir (opt_chdir_path))
         die_with_error ("Can't chdir to %s", opt_chdir_path);
       new_cwd = opt_chdir_path;
     }
   else if (chdir (old_cwd) == 0)
     {
        
       new_cwd = old_cwd;
     }
   else
     {
        
       const char *home = getenv ("HOME");
       if (home != NULL &&
           chdir (home) == 0)
         new_cwd = home;
     }
   xsetenv ("PWD", new_cwd, 1);
   free (old_cwd);
 
   __debug__ (("forking for child\n"));
 
   if (opt_unshare_pid || lock_files != NULL || opt_sync_fd != -1)
     {
        
 
       pid = fork ();
       if (pid == -1)
         die_with_error ("Can't fork for pid 1");
 
       if (pid != 0)
         {
            
           {
             int dont_close[3];
             int j = 0;
             if (event_fd != -1)
               dont_close[j++] = event_fd;
             if (opt_sync_fd != -1)
               dont_close[j++] = opt_sync_fd;
             dont_close[j++] = -1;
             fdwalk (proc_fd, close_extra_fds, dont_close);
           }
 
           return do_init (event_fd, pid);
         }
     }
 
   __debug__ (("launch executable %s\n", argv[0]));
 
   if (proc_fd != -1)
     close (proc_fd);
 
   if (opt_sync_fd != -1)
     close (opt_sync_fd);
 
     
    unblock_sigchild ();
  
//   if (setsid () == (pid_t) -1)
//     die_with_error ("setsid");
// 
    if (label_exec (opt_exec_label) == -1)
      die_with_error ("label_exec %s", argv[0]);
  
   if (execvp (argv[0], argv) == -1)
     die_with_error ("execvp %s", argv[0]);
 
   return 0;
 }