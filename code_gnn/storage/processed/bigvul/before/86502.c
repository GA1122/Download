modify_environment (const struct passwd *pw, const char *shell)
{
  if (simulate_login)
    {
       
      char *term = getenv ("TERM");
      if (term)
	term = xstrdup (term);
      environ = xmalloc ((6 + !!term) * sizeof (char *));
      environ[0] = NULL;
      if (term) {
	xsetenv ("TERM", term, 1);
	free(term);
      }
      xsetenv ("HOME", pw->pw_dir, 1);
      if (shell)
	xsetenv ("SHELL", shell, 1);
      xsetenv ("USER", pw->pw_name, 1);
      xsetenv ("LOGNAME", pw->pw_name, 1);
      set_path(pw);
    }
  else
    {
       
      if (change_environment)
        {
          xsetenv ("HOME", pw->pw_dir, 1);
	  if (shell)
            xsetenv ("SHELL", shell, 1);
	  if (getlogindefs_bool ("ALWAYS_SET_PATH", 0))
	    set_path(pw);

          if (pw->pw_uid)
            {
              xsetenv ("USER", pw->pw_name, 1);
              xsetenv ("LOGNAME", pw->pw_name, 1);
            }
        }
    }

  export_pamenv ();
}
