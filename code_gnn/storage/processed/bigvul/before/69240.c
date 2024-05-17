switch_to_user_with_privs (void)
{
  if (!is_privileged)
    return;

   
  if (prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0) < 0)
    die_with_error ("prctl(PR_SET_KEEPCAPS) failed");

  if (setuid (opt_sandbox_uid) < 0)
    die_with_error ("unable to drop root uid");

   
  set_required_caps ();
}
