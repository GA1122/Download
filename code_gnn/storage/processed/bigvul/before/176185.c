media_status_t AMediaCodec_start(AMediaCodec *mData) {
 status_t ret =  mData->mCodec->start();
 if (ret != OK) {
 return translate_error(ret);
 }
    mData->mActivityNotification = new AMessage(kWhatActivityNotify, mData->mHandler);
    mData->mActivityNotification->setInt32("generation", mData->mGeneration);
    requestActivityNotification(mData);
 return AMEDIA_OK;
}
