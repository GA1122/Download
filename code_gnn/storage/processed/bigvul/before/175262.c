dispatchCdmaSms(Parcel &p, RequestInfo *pRI) {
    RIL_CDMA_SMS_Message rcsm;

    RLOGD("dispatchCdmaSms");
 if (NO_ERROR != constructCdmaSms(p, pRI, rcsm)) {
 goto invalid;
 }

    CALL_ONREQUEST(pRI->pCI->requestNumber, &rcsm, sizeof(rcsm),pRI, pRI->socket_id);

#ifdef MEMSET_FREED
    memset(&rcsm, 0, sizeof(rcsm));
#endif

 return;

invalid:
    invalidCommandBlock(pRI);
 return;
}
