void uipc_stop_main_server_thread(void)
{
  
    UIPC_LOCK();
    uipc_main.running = 0;
    uipc_wakeup_locked();
    UIPC_UNLOCK();

  
  
 if (uipc_main.tid)
        pthread_join(uipc_main.tid, NULL);
}
