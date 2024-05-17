export_pamenv (void)
{
  char **env;

   
  env = pam_getenvlist (pamh);
  while (env && *env)
    {
      if (putenv (*env) != 0)
	err (EXIT_FAILURE, NULL);
      env++;
    }
}
