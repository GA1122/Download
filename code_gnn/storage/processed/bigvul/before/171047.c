void RilSapSocket::dispatchRequest(MsgHeader *req) {
 SapSocketRequest* currRequest=(SapSocketRequest*)malloc(sizeof(SapSocketRequest));
 if (!currRequest) {
        RLOGE("dispatchRequest: OOM");
        free(req);
 return;
 }
    currRequest->token = req->token;
    currRequest->curr = req;
    currRequest->p_next = NULL;
    currRequest->socketId = id;

    pendingResponseQueue.enqueue(currRequest);

 if (uimFuncs) {
        RLOGI("[%d] > SAP REQUEST type: %d. id: %d. error: %d",
        req->token,
        req->type,
        req->id,
        req->error );

#if defined(ANDROID_MULTI_SIM)
        uimFuncs->onRequest(req->id, req->payload->bytes, req->payload->size, currRequest, id);
#else
        uimFuncs->onRequest(req->id, req->payload->bytes, req->payload->size, currRequest);
#endif
 }
}
