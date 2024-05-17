static int auth_init(void) {
    
  pr_help_add(C_USER, _("<sp> username"), TRUE);
  pr_help_add(C_PASS, _("<sp> password"), TRUE);
  pr_help_add(C_ACCT, _("is not implemented"), FALSE);
  pr_help_add(C_REIN, _("is not implemented"), FALSE);

   
  set_auth_check(auth_cmd_chk_cb);

  return 0;
}
