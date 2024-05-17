static void dispatchCarrierRestrictions(Parcel &p, RequestInfo *pRI) {
    RIL_CarrierRestrictions cr;
    RIL_Carrier * allowed_carriers = NULL;
    RIL_Carrier * excluded_carriers = NULL;
 int32_t t;
 status_t status;

    memset(&cr, 0, sizeof(RIL_CarrierRestrictions));

 if (s_callbacks.version < 14) {
        RLOGE("Unsuppoted RIL version %d, min version expected %d",
              s_callbacks.version, 14);
        RIL_onRequestComplete(pRI, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
 return;
 }

    status = p.readInt32(&t);
 if (status != NO_ERROR) {
 goto invalid;
 }
    allowed_carriers = (RIL_Carrier *)calloc(t, sizeof(RIL_Carrier));
 if (allowed_carriers == NULL) {
        RLOGE("Memory allocation failed for request %s", requestToString(pRI->pCI->requestNumber));
 goto exit;
 }
    cr.len_allowed_carriers = t;
    cr.allowed_carriers = allowed_carriers;

    status = p.readInt32(&t);
 if (status != NO_ERROR) {
 goto invalid;
 }
    excluded_carriers = (RIL_Carrier *)calloc(t, sizeof(RIL_Carrier));
 if (excluded_carriers == NULL) {
        RLOGE("Memory allocation failed for request %s", requestToString(pRI->pCI->requestNumber));
 goto exit;
 }
    cr.len_excluded_carriers = t;
    cr.excluded_carriers = excluded_carriers;

    startRequest;
    appendPrintBuf("%s len_allowed_carriers:%d, len_excluded_carriers:%d,",
                   printBuf, cr.len_allowed_carriers, cr.len_excluded_carriers);

    appendPrintBuf("%s allowed_carriers:", printBuf);
 for (int32_t i = 0; i < cr.len_allowed_carriers; i++) {
        RIL_Carrier *p_cr = allowed_carriers + i;
        p_cr->mcc = strdupReadString(p);
        p_cr->mnc = strdupReadString(p);
        status = p.readInt32(&t);
        p_cr->match_type = static_cast<RIL_CarrierMatchType>(t);
 if (status != NO_ERROR) {
 goto invalid;
 }
        p_cr->match_data = strdupReadString(p);
        appendPrintBuf("%s [%d mcc:%s, mnc:%s, match_type:%d, match_data:%s],",
                       printBuf, i, p_cr->mcc, p_cr->mnc, p_cr->match_type, p_cr->match_data);
 }

 for (int32_t i = 0; i < cr.len_excluded_carriers; i++) {
        RIL_Carrier *p_cr = excluded_carriers + i;
        p_cr->mcc = strdupReadString(p);
        p_cr->mnc = strdupReadString(p);
        status = p.readInt32(&t);
        p_cr->match_type = static_cast<RIL_CarrierMatchType>(t);
 if (status != NO_ERROR) {
 goto invalid;
 }
        p_cr->match_data = strdupReadString(p);
        appendPrintBuf("%s [%d mcc:%s, mnc:%s, match_type:%d, match_data:%s],",
                       printBuf, i, p_cr->mcc, p_cr->mnc, p_cr->match_type, p_cr->match_data);
 }

    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    CALL_ONREQUEST(pRI->pCI->requestNumber,
 &cr,
 sizeof(RIL_CarrierRestrictions),
                pRI, pRI->socket_id);

 goto exit;

invalid:
    invalidCommandBlock(pRI);
    RIL_onRequestComplete(pRI, RIL_E_INVALID_ARGUMENTS, NULL, 0);
exit:
 if (allowed_carriers != NULL) {
        free(allowed_carriers);
 }
 if (excluded_carriers != NULL) {
        free(excluded_carriers);
 }
 return;
}
