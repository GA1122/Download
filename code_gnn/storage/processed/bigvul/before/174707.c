const char* MediaMetadataRetriever::extractMetadata(int keyCode)
{
    ALOGV("extractMetadata(%d)", keyCode);
 Mutex::Autolock _l(mLock);
 if (mRetriever == 0) {
        ALOGE("retriever is not initialized");
 return NULL;
 }
 return mRetriever->extractMetadata(keyCode);
}
