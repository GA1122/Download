dispatchString (Parcel& p, RequestInfo *pRI) {
 status_t status;
 size_t datalen;
 size_t stringlen;
 char *string8 = NULL;

    string8 = strdupReadString(p);

    startRequest;
    appendPrintBuf("%s%s", printBuf, string8);
    closeRequest;
    printRequest(pRI->token, pRI->pCI->requestNumber);

    CALL_ONREQUEST(pRI->pCI->requestNumber, string8,
 sizeof(char *), pRI, pRI->socket_id);

#ifdef MEMSET_FREED
    memsetString(string8);
#endif

    free(string8);
 return;
invalid:
    invalidCommandBlock(pRI);
 return;
}
