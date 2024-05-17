processCommandBuffer(void *buffer, size_t buflen, RIL_SOCKET_ID socket_id) {
 Parcel p;
 status_t status;
 int32_t request;
 int32_t token;
 RequestInfo *pRI;
 int ret;
  
  
 pthread_mutex_t* pendingRequestsMutexHook = &s_pendingRequestsMutex;
  
 RequestInfo**    pendingRequestsHook = &s_pendingRequests;

    p.setData((uint8_t *) buffer, buflen);

    status = p.readInt32(&request);
    status = p.readInt32 (&token);

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

 if (status != NO_ERROR) {
        RLOGE("invalid request block");
 return 0;
 }

 if (request == RIL_RESPONSE_ACKNOWLEDGEMENT) {
        releaseWakeLock();
 return 0;
 }

 if (request < 1 || request >= (int32_t)NUM_ELEMS(s_commands)) {
 Parcel pErr;
        RLOGE("unsupported request code %d token %d", request, token);
        pErr.writeInt32 (RESPONSE_SOLICITED);
        pErr.writeInt32 (token);
        pErr.writeInt32 (RIL_E_GENERIC_FAILURE);

        sendResponse(pErr, socket_id);
 return 0;
 }

    pRI = (RequestInfo *)calloc(1, sizeof(RequestInfo));
 if (pRI == NULL) {
        RLOGE("Memory allocation failed for request %s", requestToString(request));
 return 0;
 }

    pRI->token = token;
    pRI->pCI = &(s_commands[request]);
    pRI->socket_id = socket_id;

    ret = pthread_mutex_lock(pendingRequestsMutexHook);
    assert (ret == 0);

    pRI->p_next = *pendingRequestsHook;
 *pendingRequestsHook = pRI;

    ret = pthread_mutex_unlock(pendingRequestsMutexHook);
    assert (ret == 0);

 

    pRI->pCI->dispatchFunction(p, pRI);

 return 0;
}
