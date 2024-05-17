static void auth_exit_ev(const void *event_data, void *user_data) {
   
  (void) pr_close_scoreboard(FALSE);
}
