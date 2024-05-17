checkAndDequeueRequestInfoIfAck(struct RequestInfo *pRI, bool isAck) {
 int ret = 0;
  
 pthread_mutex_t* pendingRequestsMutexHook = &s_pendingRequestsMutex;
  
 RequestInfo ** pendingRequestsHook = &s_pendingRequests;

 if (pRI == NULL) {
 return 0;
 }

#if (SIM_COUNT >= 2)
 if (pRI->socket_id == RIL_SOCKET_2) {
        pendingRequestsMutexHook = &s_pendingRequestsMutex_socket2;
        pendingRequestsHook = &s_pendingRequests_socket2;
 }
#if (SIM_COUNT >= 3)
 if (pRI->socket_id == RIL_SOCKET_3) {
            pendingRequestsMutexHook = &s_pendingRequestsMutex_socket3;
            pendingRequestsHook = &s_pendingRequests_socket3;
 }
#endif
#if (SIM_COUNT >= 4)
 if (pRI->socket_id == RIL_SOCKET_4) {
        pendingRequestsMutexHook = &s_pendingRequestsMutex_socket4;
        pendingRequestsHook = &s_pendingRequests_socket4;
 }
#endif
#endif
    pthread_mutex_lock(pendingRequestsMutexHook);

 for(RequestInfo **ppCur = pendingRequestsHook
 ; *ppCur != NULL
 ; ppCur = &((*ppCur)->p_next)
 ) {
 if (pRI == *ppCur) {
            ret = 1;
 if (isAck) {  
 if (pRI->wasAckSent == 1) {
                    RLOGD("Ack was already sent for %s", requestToString(pRI->pCI->requestNumber));
 } else {
                    pRI->wasAckSent = 1;
 }
 } else {
 *ppCur = (*ppCur)->p_next;
 }
 break;
 }
 }

    pthread_mutex_unlock(pendingRequestsMutexHook);

 return ret;
}
