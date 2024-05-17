media_status_t AMediaCodec_setNotificationCallback(AMediaCodec *mData, OnCodecEvent callback,
 void *userdata) {
    mData->mCallback = callback;
    mData->mCallbackUserData = userdata;
 return AMEDIA_OK;
}
