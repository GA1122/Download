media_status_t AMediaCodec_setAsyncNotifyCallback(
 AMediaCodec *mData,
 AMediaCodecOnAsyncNotifyCallback callback,
 void *userdata) {
 if (mData->mAsyncNotify == NULL && userdata != NULL) {
        mData->mAsyncNotify = new AMessage(kWhatAsyncNotify, mData->mHandler);
 status_t err = mData->mCodec->setCallback(mData->mAsyncNotify);
 if (err != OK) {
            ALOGE("setAsyncNotifyCallback: err(%d), failed to set async callback", err);
 return translate_error(err);
 }
 }

 Mutex::Autolock _l(mData->mAsyncCallbackLock);
    mData->mAsyncCallback = callback;
    mData->mAsyncCallbackUserData = userdata;

 return AMEDIA_OK;
}
