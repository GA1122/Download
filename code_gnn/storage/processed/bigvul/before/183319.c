 static int create_problem_dir(GHashTable *problem_info, unsigned pid)
 {
      
     if (g_settings_nMaxCrashReportsSize > 0)
     {
         if (low_free_space(g_settings_nMaxCrashReportsSize, g_settings_dump_location))
             exit(1);
     }
 
      
 
     gchar *dir_basename = g_hash_table_lookup(problem_info, "basename");
     if (!dir_basename)
         dir_basename = g_hash_table_lookup(problem_info, FILENAME_TYPE);
 
     char *path = xasprintf("%s/%s-%s-%u.new",
                            g_settings_dump_location,
                            dir_basename,
                            iso_date_string(NULL),
                            pid);
 
      
     g_hash_table_remove(problem_info, "basename");
 
       
    struct dump_dir *dd = dd_create(path, client_uid, DEFAULT_DUMP_DIR_MODE);
//     struct dump_dir *dd = dd_create(path, g_settings_privatereports ? 0 : client_uid, DEFAULT_DUMP_DIR_MODE);
      if (!dd)
      {
          error_msg_and_die("Error creating problem directory '%s'", path);
     }
 
     dd_create_basic_files(dd, client_uid, NULL);
     dd_save_text(dd, FILENAME_ABRT_VERSION, VERSION);
 
     gpointer gpkey = g_hash_table_lookup(problem_info, FILENAME_CMDLINE);
     if (!gpkey)
     {
          
         char *cmdline = get_cmdline(pid);
         if (cmdline)
         {
             dd_save_text(dd, FILENAME_CMDLINE, cmdline);
             free(cmdline);
         }
     }
 
      
     char uid_str[sizeof(long) * 3 + 2];
     sprintf(uid_str, "%lu", (long)client_uid);
     dd_save_text(dd, FILENAME_UID, uid_str);
 
     GHashTableIter iter;
     gpointer gpvalue;
     g_hash_table_iter_init(&iter, problem_info);
     while (g_hash_table_iter_next(&iter, &gpkey, &gpvalue))
     {
         dd_save_text(dd, (gchar *) gpkey, (gchar *) gpvalue);
     }
 
     dd_close(dd);
 
      
     g_hash_table_destroy(problem_info);
 
      
     char *newpath = xstrndup(path, strlen(path) - strlen(".new"));
     if (rename(path, newpath) == 0)
         strcpy(path, newpath);
     free(newpath);
 
     log_notice("Saved problem directory of pid %u to '%s'", pid, path);
 
      
     printf("HTTP/1.1 201 Created\r\n\r\n");
     fflush(NULL);
     close(STDOUT_FILENO);
     xdup2(STDERR_FILENO, STDOUT_FILENO);  
 
      
     if (g_settings_nMaxCrashReportsSize > 0)
     {
         trim_problem_dirs(g_settings_dump_location, g_settings_nMaxCrashReportsSize * (double)(1024*1024), path);
     }
 
     run_post_create(path);
 
      
     exit(0);
 }