  static int get_default_root(pool *p, int allow_symlinks, char **root) {
    config_rec *c = NULL;
    char *dir = NULL;
   int res;
 
   c = find_config(main_server->conf, CONF_PARAM, "DefaultRoot", FALSE);
   while (c) {
     pr_signals_handle();
 
      
     if (c->argc < 2) {
       dir = c->argv[0];
       break;
     }
 
     res = pr_expr_eval_group_and(((char **) c->argv)+1);
     if (res) {
       dir = c->argv[0];
       break;
     }
 
     c = find_config_next(c, c->next, CONF_PARAM, "DefaultRoot", FALSE);
   }
 
   if (dir) {
     char *new_dir;
 
      
     new_dir = path_subst_uservar(p, &dir);
     if (new_dir != NULL) {
       dir = new_dir;
     }
 
     if (strncmp(dir, "/", 2) == 0) {
       dir = NULL;
 
     } else {
       char *realdir;
       int xerrno = 0;
  
        if (allow_symlinks == FALSE) {
          char *path, target_path[PR_TUNABLE_PATH_MAX + 1];
        struct stat st;
          size_t pathlen;
  
           
 
         path = dir;
         if (*path != '/') {
           if (*path == '~') {
             if (pr_fs_interpolate(dir, target_path,
                 sizeof(target_path)-1) < 0) {
               return -1;
             }
 
             path = target_path;
           }
         }
 
          
         pathlen = strlen(path);
         if (pathlen > 1 &&
             path[pathlen-1] == '/') {
            path[pathlen-1] = '\0';
          }
  
        pr_fs_clear_cache();
        res = pr_fsio_lstat(path, &st);
//         res = is_symlink_path(p, path, pathlen);
          if (res < 0) {
          xerrno = errno;
          pr_log_pri(PR_LOG_WARNING, "error: unable to check %s: %s", path,
            strerror(xerrno));
          errno = xerrno;
          return -1;
        }
//           if (errno == EPERM) {
//             pr_log_pri(PR_LOG_WARNING, "error: DefaultRoot %s is a symlink "
//               "(denied by AllowChrootSymlinks config)", path);
//           }
  
        if (S_ISLNK(st.st_mode)) {
          pr_log_pri(PR_LOG_WARNING,
            "error: DefaultRoot %s is a symlink (denied by AllowChrootSymlinks "
            "config)", path);
            errno = EPERM;
            return -1;
          }
       }
 
        
 
       PRIVS_USER
       realdir = dir_realpath(p, dir);
       xerrno = errno;
       PRIVS_RELINQUISH
 
       if (realdir) {
         dir = realdir;
 
       } else {
          
         char interp_dir[PR_TUNABLE_PATH_MAX + 1];
 
         memset(interp_dir, '\0', sizeof(interp_dir));
         (void) pr_fs_interpolate(dir, interp_dir, sizeof(interp_dir)-1); 
 
         pr_log_pri(PR_LOG_NOTICE,
           "notice: unable to use DefaultRoot '%s' [resolved to '%s']: %s",
           dir, interp_dir, strerror(xerrno));
 
         errno = xerrno;
       }
     }
   }
 
   *root = dir;
   return 0;
 }