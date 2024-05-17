RIL_onRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen) {
 RequestInfo *pRI;
 int ret;
 int fd;
 size_t errorOffset;
    RIL_SOCKET_ID socket_id = RIL_SOCKET_1;

    pRI = (RequestInfo *)t;

 if (!checkAndDequeueRequestInfoIfAck(pRI, false)) {
        RLOGE ("RIL_onRequestComplete: invalid RIL_Token");
 return;
 }

    socket_id = pRI->socket_id;
    fd = findFd(socket_id);

#if VDBG
    RLOGD("RequestComplete, %s", rilSocketIdToString(socket_id));
#endif

 if (pRI->local > 0) {
        RLOGD("C[locl]< %s", requestToString(pRI->pCI->requestNumber));

 goto done;
 }

    appendPrintBuf("[%04d]< %s",
        pRI->token, requestToString(pRI->pCI->requestNumber));

 if (pRI->cancelled == 0) {
 Parcel p;

 if (s_callbacks.version >= 13 && pRI->wasAckSent == 1) {
            p.writeInt32 (RESPONSE_SOLICITED_ACK_EXP);
            grabPartialWakeLock();
 } else {
            p.writeInt32 (RESPONSE_SOLICITED);
 }
        p.writeInt32 (pRI->token);
        errorOffset = p.dataPosition();

        p.writeInt32 (e);

 if (response != NULL) {
            ret = pRI->pCI->responseFunction(p, response, responselen);

  
 if (ret != 0) {
                RLOGE ("responseFunction error, ret %d", ret);
                p.setDataPosition(errorOffset);
                p.writeInt32 (ret);
 }
 }

 if (e != RIL_E_SUCCESS) {
            appendPrintBuf("%s fails by %s", printBuf, failCauseToString(e));
 }

 if (fd < 0) {
            RLOGD ("RIL onRequestComplete: Command channel closed");
 }
        sendResponse(p, socket_id);
 }

done:
    free(pRI);
}
