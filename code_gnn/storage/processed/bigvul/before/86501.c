log_syslog(struct passwd const *pw, bool successful)
{
  const char *new_user, *old_user, *tty;

  new_user = pw->pw_name;
   
  old_user = getlogin ();
  if (!old_user)
    {
       
      struct passwd *pwd = current_getpwuid();
      old_user = pwd ? pwd->pw_name : "";
    }

  if (get_terminal_name(NULL, &tty, NULL) != 0 || !tty)
    tty = "none";

  openlog (program_invocation_short_name, 0 , LOG_AUTH);
  syslog (LOG_NOTICE, "%s(to %s) %s on %s",
	  successful ? "" :
	  su_mode == RUNUSER_MODE ? "FAILED RUNUSER " : "FAILED SU ",
	  new_user, old_user, tty);
  closelog ();
}
