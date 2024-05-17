static void ensure_open_passwd(pool *p) {
   
  pr_auth_setpwent(p);
  pr_auth_setgrent(p);

   
  pr_auth_getpwent(p);
  pr_auth_getgrent(p);
}
