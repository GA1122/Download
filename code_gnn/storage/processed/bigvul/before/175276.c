dispatchRaw(Parcel &p, RequestInfo *pRI) {
 int32_t len;
 status_t status;
 const void *data;

    status = p.readInt32(&len);

 if (status != NO_ERROR) {
 goto invalid;
 }

 if (((int)len) == -1) {
        data = NULL;
        len = 0;
 }

    data = p.readInplace(len);

    startRequest;
    appendPrintBuf("%sraw_size=%d", printBuf, len);
    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    CALL_ONREQUEST(pRI->pCI->requestNumber, const_cast<void *>(data), len, pRI, pRI->socket_id);

 return;
invalid:
    invalidCommandBlock(pRI);
 return;
}
