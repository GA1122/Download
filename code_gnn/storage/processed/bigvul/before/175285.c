static void dispatchUiccSubscripton(Parcel &p, RequestInfo *pRI) {
    RIL_SelectUiccSub uicc_sub;
 status_t status;
 int32_t  t;
    memset(&uicc_sub, 0, sizeof(uicc_sub));

    status = p.readInt32(&t);
 if (status != NO_ERROR) {
 goto invalid;
 }
    uicc_sub.slot = (int) t;

    status = p.readInt32(&t);
 if (status != NO_ERROR) {
 goto invalid;
 }
    uicc_sub.app_index = (int) t;

    status = p.readInt32(&t);
 if (status != NO_ERROR) {
 goto invalid;
 }
    uicc_sub.sub_type = (RIL_SubscriptionType) t;

    status = p.readInt32(&t);
 if (status != NO_ERROR) {
 goto invalid;
 }
    uicc_sub.act_status = (RIL_UiccSubActStatus) t;

    startRequest;
    appendPrintBuf("slot=%d, app_index=%d, act_status = %d", uicc_sub.slot, uicc_sub.app_index,
            uicc_sub.act_status);
    RLOGD("dispatchUiccSubscription, slot=%d, app_index=%d, act_status = %d", uicc_sub.slot,
            uicc_sub.app_index, uicc_sub.act_status);
    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    CALL_ONREQUEST(pRI->pCI->requestNumber, &uicc_sub, sizeof(uicc_sub), pRI, pRI->socket_id);

#ifdef MEMSET_FREED
    memset(&uicc_sub, 0, sizeof(uicc_sub));
#endif
 return;

invalid:
    invalidCommandBlock(pRI);
 return;
}
