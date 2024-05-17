static void onCommandsSocketClosed(RIL_SOCKET_ID socket_id) {
 int ret;
 RequestInfo *p_cur;
  
 pthread_mutex_t * pendingRequestsMutexHook = &s_pendingRequestsMutex;
  
 RequestInfo **    pendingRequestsHook = &s_pendingRequests;

#if (SIM_COUNT >= 2)
 if (socket_id == RIL_SOCKET_2) {
        pendingRequestsMutexHook = &s_pendingRequestsMutex_socket2;
        pendingRequestsHook = &s_pendingRequests_socket2;
 }
#if (SIM_COUNT >= 3)
 else if (socket_id == RIL_SOCKET_3) {
        pendingRequestsMutexHook = &s_pendingRequestsMutex_socket3;
        pendingRequestsHook = &s_pendingRequests_socket3;
 }
#endif
#if (SIM_COUNT >= 4)
 else if (socket_id == RIL_SOCKET_4) {
        pendingRequestsMutexHook = &s_pendingRequestsMutex_socket4;
        pendingRequestsHook = &s_pendingRequests_socket4;
 }
#endif
#endif
  
    ret = pthread_mutex_lock(pendingRequestsMutexHook);
    assert (ret == 0);

    p_cur = *pendingRequestsHook;

 for (p_cur = *pendingRequestsHook
 ; p_cur != NULL
 ; p_cur  = p_cur->p_next
 ) {
        p_cur->cancelled = 1;
 }

    ret = pthread_mutex_unlock(pendingRequestsMutexHook);
    assert (ret == 0);
}
