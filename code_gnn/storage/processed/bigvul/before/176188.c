static AMediaCodec * createAMediaCodec(const char *name, bool name_is_type, bool encoder) {
 AMediaCodec *mData = new AMediaCodec();
    mData->mLooper = new ALooper;
    mData->mLooper->setName("NDK MediaCodec_looper");
 size_t res = mData->mLooper->start(
 false,  
 true,  
            PRIORITY_AUDIO);
 if (res != OK) {
        ALOGE("Failed to start the looper");
 AMediaCodec_delete(mData);
 return NULL;
 }
 if (name_is_type) {
        mData->mCodec = android::MediaCodec::CreateByType(mData->mLooper, name, encoder);
 } else {
        mData->mCodec = android::MediaCodec::CreateByComponentName(mData->mLooper, name);
 }
 if (mData->mCodec == NULL) {  
 AMediaCodec_delete(mData);
 return NULL;
 }
    mData->mHandler = new CodecHandler(mData);
    mData->mLooper->registerHandler(mData->mHandler);
    mData->mGeneration = 1;
    mData->mRequestedActivityNotification = false;
    mData->mCallback = NULL;

    mData->mAsyncCallback = {};
    mData->mAsyncCallbackUserData = NULL;

 return mData;
}
