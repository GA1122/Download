media_status_t AMediaCodec_signalEndOfInputStream(AMediaCodec *mData) {

 if (mData == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }

 status_t err = mData->mCodec->signalEndOfInputStream();
 if (err == INVALID_OPERATION) {
 return AMEDIA_ERROR_INVALID_OPERATION;
 }

 return translate_error(err);

}
