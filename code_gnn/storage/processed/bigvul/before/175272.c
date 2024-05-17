dispatchInts (Parcel &p, RequestInfo *pRI) {
 int32_t count;
 status_t status;
 size_t datalen;
 int *pInts;

    status = p.readInt32 (&count);

 if (status != NO_ERROR || count <= 0) {
 goto invalid;
 }

    datalen = sizeof(int) * count;
    pInts = (int *)calloc(count, sizeof(int));
 if (pInts == NULL) {
        RLOGE("Memory allocation failed for request %s", requestToString(pRI->pCI->requestNumber));
 return;
 }

    startRequest;
 for (int i = 0 ; i < count ; i++) {
 int32_t t;

        status = p.readInt32(&t);
        pInts[i] = (int)t;
        appendPrintBuf("%s%d,", printBuf, t);

 if (status != NO_ERROR) {
            free(pInts);
 goto invalid;
 }
 }
   removeLastChar;
   closeRequest;
   printRequest(pRI->token, pRI->pCI->requestNumber);

   CALL_ONREQUEST(pRI->pCI->requestNumber, const_cast<int *>(pInts),
                       datalen, pRI, pRI->socket_id);

#ifdef MEMSET_FREED
    memset(pInts, 0, datalen);
#endif
    free(pInts);
 return;
invalid:
    invalidCommandBlock(pRI);
 return;
}
