void RilSapSocket::dispatchDisconnect(MsgHeader *req) {
 SapSocketRequest* currRequest=(SapSocketRequest*)malloc(sizeof(SapSocketRequest));
 if (!currRequest) {
        RLOGE("dispatchDisconnect: OOM");
        free(req->payload);
        free(req);
 return;
 }
    currRequest->token = -1;
    currRequest->curr = req;
    currRequest->p_next = NULL;
    currRequest->socketId = (RIL_SOCKET_ID)99;

    RLOGD("Sending disconnect on command close!");

#if defined(ANDROID_MULTI_SIM)
    uimFuncs->onRequest(req->id, req->payload->bytes, req->payload->size, currRequest, id);
#else
    uimFuncs->onRequest(req->id, req->payload->bytes, req->payload->size, currRequest);
#endif
}
