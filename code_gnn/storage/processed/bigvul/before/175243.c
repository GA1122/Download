RIL_onRequestAck(RIL_Token t) {
 RequestInfo *pRI;
 int ret, fd;

 size_t errorOffset;
    RIL_SOCKET_ID socket_id = RIL_SOCKET_1;

    pRI = (RequestInfo *)t;

 if (!checkAndDequeueRequestInfoIfAck(pRI, true)) {
        RLOGE ("RIL_onRequestAck: invalid RIL_Token");
 return;
 }

    socket_id = pRI->socket_id;
    fd = findFd(socket_id);

#if VDBG
    RLOGD("Request Ack, %s", rilSocketIdToString(socket_id));
#endif

    appendPrintBuf("Ack [%04d]< %s", pRI->token, requestToString(pRI->pCI->requestNumber));

 if (pRI->cancelled == 0) {
 Parcel p;

        p.writeInt32 (RESPONSE_SOLICITED_ACK);
        p.writeInt32 (pRI->token);

 if (fd < 0) {
            RLOGD ("RIL onRequestComplete: Command channel closed");
 }

        sendResponse(p, socket_id);
 }
}
