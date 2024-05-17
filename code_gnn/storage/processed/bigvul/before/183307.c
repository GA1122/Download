 int main(int argc, char** argv)
 {
      
     int fd = xopen("/dev/null", O_RDWR);
     while (fd < 2)
 	fd = xdup(fd);
     if (fd > 2)
 	close(fd);
 
     if (argc < 8)
     {
          
          
         error_msg_and_die("Usage: %s SIGNO CORE_SIZE_LIMIT PID UID GID TIME BINARY_NAME [HOSTNAME]", argv[0]);
     }
 
      
     if (strchr(argv[1], ' '))
     {
         int i;
         for (i = 1; argv[i]; i++)
         {
             strchrnul(argv[i], ' ')[0] = '\0';
         }
     }
 
     logmode = LOGMODE_JOURNAL;
 
      
     load_abrt_conf();
      
     bool setting_MakeCompatCore;
     bool setting_SaveBinaryImage;
     {
         map_string_t *settings = new_map_string();
         load_abrt_plugin_conf_file("CCpp.conf", settings);
         const char *value;
         value = get_map_string_item_or_NULL(settings, "MakeCompatCore");
         setting_MakeCompatCore = value && string_to_bool(value);
         value = get_map_string_item_or_NULL(settings, "SaveBinaryImage");
         setting_SaveBinaryImage = value && string_to_bool(value);
         value = get_map_string_item_or_NULL(settings, "VerboseLog");
         if (value)
             g_verbose = xatoi_positive(value);
         free_map_string(settings);
     }
 
     errno = 0;
     const char* signal_str = argv[1];
     int signal_no = xatoi_positive(signal_str);
     off_t ulimit_c = strtoull(argv[2], NULL, 10);
     if (ulimit_c < 0)  
     {
          
         ulimit_c = ~((off_t)1 << (sizeof(off_t)*8-1));
     }
     const char *pid_str = argv[3];
     pid_t pid = xatoi_positive(argv[3]);
     uid_t uid = xatoi_positive(argv[4]);
     if (errno || pid <= 0)
     {
         perror_msg_and_die("PID '%s' or limit '%s' is bogus", argv[3], argv[2]);
     }
 
     {
         char *s = xmalloc_fopen_fgetline_fclose(VAR_RUN"/abrt/saved_core_pattern");
          
         if (s && s[0] != '|')
             core_basename = s;
         else
             free(s);
     }
 
     struct utsname uts;
     if (!argv[8])  
     {
         uname(&uts);
         argv[8] = uts.nodename;
     }
 
     char path[PATH_MAX];
 
     int src_fd_binary = -1;
     char *executable = get_executable(pid, setting_SaveBinaryImage ? &src_fd_binary : NULL);
     if (executable && strstr(executable, "/abrt-hook-ccpp"))
     {
         error_msg_and_die("PID %lu is '%s', not dumping it to avoid recursion",
                         (long)pid, executable);
     }
 
     user_pwd = get_cwd(pid);  
     log_notice("user_pwd:'%s'", user_pwd);
 
     sprintf(path, "/proc/%lu/status", (long)pid);
     proc_pid_status = xmalloc_xopen_read_close(path,   NULL);
 
     uid_t fsuid = uid;
     uid_t tmp_fsuid = get_fsuid();
     int suid_policy = dump_suid_policy();
     if (tmp_fsuid != uid)
     {
          
         fsuid = 0;
         if (suid_policy == DUMP_SUID_UNSAFE)
         {
             fsuid = tmp_fsuid;
         }
     }
 
      
     if (setting_MakeCompatCore && ulimit_c != 0)
          
         user_core_fd = open_user_core(uid, fsuid, pid, &argv[1]);
 
     if (executable == NULL)
     {
          
         error_msg("Can't read /proc/%lu/exe link", (long)pid);
         goto create_user_core;
     }
 
     const char *signame = NULL;
     switch (signal_no)
     {
         case SIGILL : signame = "ILL" ; break;
         case SIGFPE : signame = "FPE" ; break;
         case SIGSEGV: signame = "SEGV"; break;
         case SIGBUS : signame = "BUS" ; break;  
         case SIGABRT: signame = "ABRT"; break;  
         case SIGTRAP: signame = "TRAP"; break;  
         default: goto create_user_core;  
     }
 
     if (!daemon_is_ok())
     {
          
         log("abrtd is not running. If it crashed, "
             "/proc/sys/kernel/core_pattern contains a stale value, "
             "consider resetting it to 'core'"
         );
         goto create_user_core;
     }
 
     if (g_settings_nMaxCrashReportsSize > 0)
     {
          
         if (low_free_space(g_settings_nMaxCrashReportsSize, g_settings_dump_location))
             goto create_user_core;
     }
 
      
     snprintf(path, sizeof(path), "%s/last-ccpp", g_settings_dump_location);
     if (check_recent_crash_file(path, executable))
     {
          
         goto create_user_core;
     }
 
     const char *last_slash = strrchr(executable, '/');
     if (last_slash && strncmp(++last_slash, "abrt", 4) == 0)
     {
          
         snprintf(path, sizeof(path), "%s/%s-coredump", g_settings_dump_location, last_slash);
         int abrt_core_fd = xopen3(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
         off_t core_size = copyfd_eof(STDIN_FILENO, abrt_core_fd, COPYFD_SPARSE);
         if (core_size < 0 || fsync(abrt_core_fd) != 0)
         {
             unlink(path);
              
             error_msg_and_die("Error saving '%s'", path);
         }
         log("Saved core dump of pid %lu (%s) to %s (%llu bytes)", (long)pid, executable, path, (long long)core_size);
         return 0;
     }
 
     unsigned path_len = snprintf(path, sizeof(path), "%s/ccpp-%s-%lu.new",
             g_settings_dump_location, iso_date_string(NULL), (long)pid);
     if (path_len >= (sizeof(path) - sizeof("/"FILENAME_COREDUMP)))
     {
         goto create_user_core;
     }
 
      
     dd = dd_create(path, fsuid, DEFAULT_DUMP_DIR_MODE);
     if (dd)
      {
          char *rootdir = get_rootdir(pid);
  
        dd_create_basic_files(dd, fsuid, (rootdir && strcmp(rootdir, "/") != 0) ? rootdir : NULL);
//         dd_create_basic_files(dd, fsuid, NULL);
  
          char source_filename[sizeof("/proc/%lu/somewhat_long_name") + sizeof(long)*3];
          int source_base_ofs = sprintf(source_filename, "/proc/%lu/smaps", (long)pid);
         source_base_ofs -= strlen("smaps");
         char *dest_filename = concat_path_file(dd->dd_dirname, "also_somewhat_longish_name");
         char *dest_base = strrchr(dest_filename, '/') + 1;
 
 
         strcpy(source_filename + source_base_ofs, "maps");
         strcpy(dest_base, FILENAME_MAPS);
         copy_file_ext(source_filename, dest_filename, 0640, dd->dd_uid, dd->dd_gid, O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC | O_EXCL);
 
         strcpy(source_filename + source_base_ofs, "limits");
         strcpy(dest_base, FILENAME_LIMITS);
         copy_file_ext(source_filename, dest_filename, 0640, dd->dd_uid, dd->dd_gid, O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC | O_EXCL);
 
         strcpy(source_filename + source_base_ofs, "cgroup");
         strcpy(dest_base, FILENAME_CGROUP);
         copy_file_ext(source_filename, dest_filename, 0640, dd->dd_uid, dd->dd_gid, O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC | O_EXCL);
 
         strcpy(dest_base, FILENAME_OPEN_FDS);
         dump_fd_info(dest_filename, source_filename, source_base_ofs, dd->dd_uid, dd->dd_gid);
 
         free(dest_filename);
 
         dd_save_text(dd, FILENAME_ANALYZER, "CCpp");
         dd_save_text(dd, FILENAME_TYPE, "CCpp");
         dd_save_text(dd, FILENAME_EXECUTABLE, executable);
         dd_save_text(dd, FILENAME_PID, pid_str);
         dd_save_text(dd, FILENAME_PROC_PID_STATUS, proc_pid_status);
         if (user_pwd)
             dd_save_text(dd, FILENAME_PWD, user_pwd);
         if (rootdir)
         {
             if (strcmp(rootdir, "/") != 0)
                 dd_save_text(dd, FILENAME_ROOTDIR, rootdir);
         }
 
         char *reason = xasprintf("%s killed by SIG%s",
                                  last_slash, signame ? signame : signal_str);
         dd_save_text(dd, FILENAME_REASON, reason);
         free(reason);
 
         char *cmdline = get_cmdline(pid);
         dd_save_text(dd, FILENAME_CMDLINE, cmdline ? : "");
         free(cmdline);
 
         char *environ = get_environ(pid);
         dd_save_text(dd, FILENAME_ENVIRON, environ ? : "");
         free(environ);
 
         char *fips_enabled = xmalloc_fopen_fgetline_fclose("/proc/sys/crypto/fips_enabled");
         if (fips_enabled)
         {
             if (strcmp(fips_enabled, "0") != 0)
                 dd_save_text(dd, "fips_enabled", fips_enabled);
             free(fips_enabled);
         }
 
         dd_save_text(dd, FILENAME_ABRT_VERSION, VERSION);
 
         if (src_fd_binary > 0)
         {
             strcpy(path + path_len, "/"FILENAME_BINARY);
             int dst_fd = create_or_die(path);
             off_t sz = copyfd_eof(src_fd_binary, dst_fd, COPYFD_SPARSE);
             if (fsync(dst_fd) != 0 || close(dst_fd) != 0 || sz < 0)
             {
                 dd_delete(dd);
                 error_msg_and_die("Error saving '%s'", path);
             }
             close(src_fd_binary);
         }
 
         strcpy(path + path_len, "/"FILENAME_COREDUMP);
         int abrt_core_fd = create_or_die(path);
 
          
         off_t core_size = copyfd_sparse(STDIN_FILENO, abrt_core_fd, user_core_fd, ulimit_c);
         if (fsync(abrt_core_fd) != 0 || close(abrt_core_fd) != 0 || core_size < 0)
         {
             unlink(path);
             dd_delete(dd);
             if (user_core_fd >= 0)
             {
                 xchdir(user_pwd);
                 unlink(core_basename);
             }
              
             error_msg_and_die("Error writing '%s'", path);
         }
         if (user_core_fd >= 0
              
          && (fsync(user_core_fd) != 0 || close(user_core_fd) != 0
              
             || (ulimit_c == 0   || core_size > ulimit_c)
             )
         ) {
              
             xchdir(user_pwd);
             unlink(core_basename);
         }
 
  
 #if 0
          
         {
             char *java_log = xasprintf("/tmp/jvm-%lu/hs_error.log", (long)pid);
             int src_fd = open(java_log, O_RDONLY);
             free(java_log);
 
              
             if (src_fd < 0)
             {
                 java_log = xasprintf("%s/hs_err_pid%lu.log", user_pwd, (long)pid);
                 src_fd = open(java_log, O_RDONLY);
                 free(java_log);
             }
 
             if (src_fd >= 0)
             {
                 strcpy(path + path_len, "/hs_err.log");
                 int dst_fd = create_or_die(path);
                 off_t sz = copyfd_eof(src_fd, dst_fd, COPYFD_SPARSE);
                 if (close(dst_fd) != 0 || sz < 0)
                 {
                     dd_delete(dd);
                     error_msg_and_die("Error saving '%s'", path);
                 }
                 close(src_fd);
             }
         }
 #endif
 
          
         dd_close(dd);
         path[path_len] = '\0';  
         char *newpath = xstrndup(path, path_len - (sizeof(".new")-1));
         if (rename(path, newpath) == 0)
             strcpy(path, newpath);
         free(newpath);
 
         log("Saved core dump of pid %lu (%s) to %s (%llu bytes)", (long)pid, executable, path, (long long)core_size);
 
         notify_new_path(path);
 
          
         if (g_settings_nMaxCrashReportsSize > 0)
         {
              
             unsigned maxsize = g_settings_nMaxCrashReportsSize + g_settings_nMaxCrashReportsSize / 4;
             maxsize |= 63;
             trim_problem_dirs(g_settings_dump_location, maxsize * (double)(1024*1024), path);
         }
 
         free(rootdir);
         return 0;
     }
 
      
  create_user_core:
     if (user_core_fd >= 0)
     {
         off_t core_size = copyfd_size(STDIN_FILENO, user_core_fd, ulimit_c, COPYFD_SPARSE);
         if (fsync(user_core_fd) != 0 || close(user_core_fd) != 0 || core_size < 0)
         {
              
             perror_msg("Error writing '%s'", full_core_basename);
             xchdir(user_pwd);
             unlink(core_basename);
             return 1;
         }
         if (ulimit_c == 0 || core_size > ulimit_c)
         {
             xchdir(user_pwd);
             unlink(core_basename);
             return 1;
         }
         log("Saved core dump of pid %lu to %s (%llu bytes)", (long)pid, full_core_basename, (long long)core_size);
     }
 
     return 0;
 }