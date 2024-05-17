media_status_t AMediaCodec_stop(AMediaCodec *mData) {
 media_status_t ret = translate_error(mData->mCodec->stop());

    sp<AMessage> msg = new AMessage(kWhatStopActivityNotifications, mData->mHandler);
    sp<AMessage> response;
    msg->postAndAwaitResponse(&response);
    mData->mActivityNotification.clear();

 return ret;
}
