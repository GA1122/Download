 static inline int btif_hl_close_select_thread(void)
 {
 
      int result = 0;
      char sig_on = btif_hl_signal_select_exit;
      BTIF_TRACE_DEBUG("btif_hl_signal_select_exit");
    result = send(signal_fds[1], &sig_on, sizeof(sig_on), 0);
//     result = TEMP_FAILURE_RETRY(send(signal_fds[1], &sig_on, sizeof(sig_on), 0));
      if (btif_is_enabled())
      {
           
  if (select_thread_id != -1) {
             pthread_join(select_thread_id, NULL);
             select_thread_id = -1;
  }
  }
     list_free(soc_queue);
  return result;
 }