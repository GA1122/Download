static void btif_hl_thread_cleanup(){
 if (listen_s != -1)
        close(listen_s);
 if (connected_s != -1)
 {
        shutdown(connected_s, SHUT_RDWR);
        close(connected_s);
 }
    listen_s = connected_s = -1;
    BTIF_TRACE_DEBUG("hl thread cleanup");
}
