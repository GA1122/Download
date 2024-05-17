static int _define_var(pam_handle_t *pamh, VAR *var)
{
   

  char *envvar;
  int retval = PAM_SUCCESS;

  D(("Called."));
  if (asprintf(&envvar, "%s=%s", var->name, var->value) < 0) {
    pam_syslog(pamh, LOG_ERR, "out of memory");
    return PAM_BUF_ERR;
  }

  retval = pam_putenv(pamh, envvar);
  _pam_drop(envvar);
  D(("Exit."));
  return retval;
}
