void RilSapSocket::onUnsolicitedResponse(int unsolResponse, void *data, size_t datalen) {
 if (data && datalen > 0) {
 pb_bytes_array_t *payload = (pb_bytes_array_t *)calloc(1,
 sizeof(pb_bytes_array_t) + datalen);
 if (!payload) {
            RLOGE("onUnsolicitedResponse: OOM");
 return;
 }
        memcpy(payload->bytes, data, datalen);
        payload->size = datalen;
 MsgHeader rsp;
        rsp.payload = payload;
        rsp.type = MsgType_UNSOL_RESPONSE;
        rsp.id = (MsgId)unsolResponse;
        rsp.error = Error_RIL_E_SUCCESS;
        sendResponse(&rsp);
        free(payload);
 }
}
