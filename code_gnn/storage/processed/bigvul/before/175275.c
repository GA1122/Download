static void dispatchRadioCapability(Parcel &p, RequestInfo *pRI){
    RIL_RadioCapability rc;
 int32_t t;
 status_t status;

    memset (&rc, 0, sizeof(RIL_RadioCapability));

    status = p.readInt32(&t);
    rc.version = (int)t;
 if (status != NO_ERROR) {
 goto invalid;
 }

    status = p.readInt32(&t);
    rc.session= (int)t;
 if (status != NO_ERROR) {
 goto invalid;
 }

    status = p.readInt32(&t);
    rc.phase= (int)t;
 if (status != NO_ERROR) {
 goto invalid;
 }

    status = p.readInt32(&t);
    rc.rat = (int)t;
 if (status != NO_ERROR) {
 goto invalid;
 }

    status = readStringFromParcelInplace(p, rc.logicalModemUuid, sizeof(rc.logicalModemUuid));
 if (status != NO_ERROR) {
 goto invalid;
 }

    status = p.readInt32(&t);
    rc.status = (int)t;

 if (status != NO_ERROR) {
 goto invalid;
 }

    startRequest;
    appendPrintBuf("%s [version:%d, session:%d, phase:%d, rat:%d, \
            logicalModemUuid:%s, status:%d", printBuf, rc.version, rc.session,
            rc.phase, rc.rat, rc.logicalModemUuid, rc.session);

    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    CALL_ONREQUEST(pRI->pCI->requestNumber,
 &rc,
 sizeof(RIL_RadioCapability),
                pRI, pRI->socket_id);
 return;
invalid:
    invalidCommandBlock(pRI);
 return;
}
