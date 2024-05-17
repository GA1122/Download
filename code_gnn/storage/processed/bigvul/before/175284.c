dispatchStrings (Parcel &p, RequestInfo *pRI) {
 int32_t countStrings;
 status_t status;
 size_t datalen;
 char **pStrings;

    status = p.readInt32 (&countStrings);

 if (status != NO_ERROR) {
 goto invalid;
 }

    startRequest;
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

    CALL_ONREQUEST(pRI->pCI->requestNumber, pStrings, datalen, pRI, pRI->socket_id);

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

 return;
invalid:
    invalidCommandBlock(pRI);
 return;
}
