void RilSapSocket::onRequestComplete(RIL_Token t, RIL_Errno e, void *response,
 size_t response_len) {
 SapSocketRequest* request= (SapSocketRequest*)t;
 MsgHeader *hdr = request->curr;

 MsgHeader rsp;
    rsp.token = request->curr->token;
    rsp.type = MsgType_RESPONSE;
    rsp.id = request->curr->id;
    rsp.error = (Error)e;
    rsp.payload = (pb_bytes_array_t *)calloc(1, sizeof(pb_bytes_array_t) + response_len);
 if (!rsp.payload) {
        RLOGE("onRequestComplete: OOM");
 } else {
 if (response && response_len > 0) {
            memcpy(rsp.payload->bytes, response, response_len);
            rsp.payload->size = response_len;
 } else {
            rsp.payload->size = 0;
 }

        RLOGE("Token:%d, MessageId:%d", hdr->token, hdr->id);

        sendResponse(&rsp);
        free(rsp.payload);
 }

 if(!pendingResponseQueue.checkAndDequeue(hdr->id, hdr->token)) {
        RLOGE("Token:%d, MessageId:%d", hdr->token, hdr->id);
        RLOGE ("RilSapSocket::onRequestComplete: invalid Token or Message Id");
 }

    free(hdr);
}
