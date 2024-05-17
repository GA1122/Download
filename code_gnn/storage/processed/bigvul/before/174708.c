sp<IMemory> MediaMetadataRetriever::getFrameAtTime(int64_t timeUs, int option)
{
    ALOGV("getFrameAtTime: time(%" PRId64 " us) option(%d)", timeUs, option);
 Mutex::Autolock _l(mLock);
 if (mRetriever == 0) {
        ALOGE("retriever is not initialized");
 return NULL;
 }
 return mRetriever->getFrameAtTime(timeUs, option);
}
