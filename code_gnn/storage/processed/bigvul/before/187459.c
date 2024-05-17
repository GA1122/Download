 static void sync_lockstate_on_connect(btif_hh_device_t *p_dev)
 {
  int keylockstates;
 
     BTIF_TRACE_EVENT("%s: Syncing keyboard lock states after "\
  "reconnect...",__FUNCTION__);
   
     update_keyboard_lockstates(p_dev);
 
   
     keylockstates = get_keylockstates();
  if (keylockstates)
  {
 
          BTIF_TRACE_DEBUG("%s: Sending hid report to kernel "\
              "indicating lock key state 0x%x",__FUNCTION__,
              keylockstates);
        usleep(200000);
//         TEMP_FAILURE_RETRY(usleep(200000));
          toggle_os_keylockstates(p_dev->fd, keylockstates);
      }
      else
  {
         BTIF_TRACE_DEBUG("%s: NOT sending hid report to kernel "\
  "indicating lock key state 0x%x",__FUNCTION__,
             keylockstates);
  }
 }