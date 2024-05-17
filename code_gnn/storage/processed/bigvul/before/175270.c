dispatchImsGsmSms(Parcel &p, RequestInfo *pRI, uint8_t retry, int32_t messageRef) {
    RIL_IMS_SMS_Message rism;
 int32_t countStrings;
 status_t status;
 size_t datalen;
 char **pStrings;
    RLOGD("dispatchImsGsmSms: retry=%d, messageRef=%d", retry, messageRef);

    status = p.readInt32 (&countStrings);

 if (status != NO_ERROR) {
 goto invalid;
 }

    memset(&rism, 0, sizeof(rism));
    rism.tech = RADIO_TECH_3GPP;
    rism.retry = retry;
    rism.messageRef = messageRef;

    startRequest;
    appendPrintBuf("%stech=%d, retry=%d, messageRef=%d, ", printBuf,
 (int)rism.tech, (int)rism.retry, rism.messageRef);
 if (countStrings == 0) {
        pStrings = (char **)calloc(1, sizeof(char *));
 if (pStrings == NULL) {
            RLOGE("Memory allocation failed for request %s",
                    requestToString(pRI->pCI->requestNumber));
            closeRequest;
 return;
 }

        datalen = 0;
 } else if (countStrings < 0) {
        pStrings = NULL;
        datalen = 0;
 } else {
 if ((size_t)countStrings > (INT_MAX/sizeof(char *))) {
            RLOGE("Invalid value of countStrings: \n");
            closeRequest;
 return;
 }
        datalen = sizeof(char *) * countStrings;

        pStrings = (char **)calloc(countStrings, sizeof(char *));
 if (pStrings == NULL) {
            RLOGE("Memory allocation failed for request %s",
                    requestToString(pRI->pCI->requestNumber));
            closeRequest;
 return;
 }

 for (int i = 0 ; i < countStrings ; i++) {
            pStrings[i] = strdupReadString(p);
            appendPrintBuf("%s%s,", printBuf, pStrings[i]);
 }
 }
    removeLastChar;
    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    rism.message.gsmMessage = pStrings;
    CALL_ONREQUEST(pRI->pCI->requestNumber, &rism,
 sizeof(RIL_RadioTechnologyFamily)+sizeof(uint8_t)+sizeof(int32_t)
 +datalen, pRI, pRI->socket_id);

 if (pStrings != NULL) {
 for (int i = 0 ; i < countStrings ; i++) {
#ifdef MEMSET_FREED
            memsetString (pStrings[i]);
#endif
            free(pStrings[i]);
 }

#ifdef MEMSET_FREED
        memset(pStrings, 0, datalen);
#endif
        free(pStrings);
 }

#ifdef MEMSET_FREED
    memset(&rism, 0, sizeof(rism));
#endif
 return;
invalid:
    ALOGE("dispatchImsGsmSms invalid block");
    invalidCommandBlock(pRI);
 return;
}
