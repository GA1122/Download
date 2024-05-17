dispatchImsSms(Parcel &p, RequestInfo *pRI) {
 int32_t  t;
 status_t status = p.readInt32(&t);
    RIL_RadioTechnologyFamily format;
 uint8_t retry;
 int32_t messageRef;

    RLOGD("dispatchImsSms");
 if (status != NO_ERROR) {
 goto invalid;
 }
    format = (RIL_RadioTechnologyFamily) t;

    status = p.read(&retry,sizeof(retry));
 if (status != NO_ERROR) {
 goto invalid;
 }
    status = p.read(&messageRef,sizeof(messageRef));
 if (status != NO_ERROR) {
 goto invalid;
 }

 if (RADIO_TECH_3GPP == format) {
        dispatchImsGsmSms(p, pRI, retry, messageRef);
 } else if (RADIO_TECH_3GPP2 == format) {
        dispatchImsCdmaSms(p, pRI, retry, messageRef);
 } else {
        ALOGE("requestImsSendSMS invalid format value =%d", format);
 }

 return;

invalid:
    invalidCommandBlock(pRI);
 return;
}
