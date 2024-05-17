dispatchDial (Parcel &p, RequestInfo *pRI) {
    RIL_Dial dial;
    RIL_UUS_Info uusInfo;
 int32_t sizeOfDial;
 int32_t t;
 int32_t uusPresent;
 status_t status;

    RLOGD("dispatchDial");
    memset (&dial, 0, sizeof(dial));

    dial.address = strdupReadString(p);

    status = p.readInt32(&t);
    dial.clir = (int)t;

 if (status != NO_ERROR || dial.address == NULL) {
 goto invalid;
 }

 if (s_callbacks.version < 3) {  
        uusPresent = 0;
        sizeOfDial = sizeof(dial) - sizeof(RIL_UUS_Info *);
 } else {
        status = p.readInt32(&uusPresent);

 if (status != NO_ERROR) {
 goto invalid;
 }

 if (uusPresent == 0) {
            dial.uusInfo = NULL;
 } else {
 int32_t len;

            memset(&uusInfo, 0, sizeof(RIL_UUS_Info));

            status = p.readInt32(&t);
            uusInfo.uusType = (RIL_UUS_Type) t;

            status = p.readInt32(&t);
            uusInfo.uusDcs = (RIL_UUS_DCS) t;

            status = p.readInt32(&len);
 if (status != NO_ERROR) {
 goto invalid;
 }

 if (((int) len) == -1) {
                uusInfo.uusData = NULL;
                len = 0;
 } else {
                uusInfo.uusData = (char*) p.readInplace(len);
 }

            uusInfo.uusLength = len;
            dial.uusInfo = &uusInfo;
 }
        sizeOfDial = sizeof(dial);
 }

    startRequest;
    appendPrintBuf("%snum=%s,clir=%d", printBuf, dial.address, dial.clir);
 if (uusPresent) {
        appendPrintBuf("%s,uusType=%d,uusDcs=%d,uusLen=%d", printBuf,
                dial.uusInfo->uusType, dial.uusInfo->uusDcs,
                dial.uusInfo->uusLength);
 }
    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    CALL_ONREQUEST(pRI->pCI->requestNumber, &dial, sizeOfDial, pRI, pRI->socket_id);

#ifdef MEMSET_FREED
    memsetString (dial.address);
#endif

    free (dial.address);

#ifdef MEMSET_FREED
    memset(&uusInfo, 0, sizeof(RIL_UUS_Info));
    memset(&dial, 0, sizeof(dial));
#endif

 return;
invalid:
    invalidCommandBlock(pRI);
 return;
}
