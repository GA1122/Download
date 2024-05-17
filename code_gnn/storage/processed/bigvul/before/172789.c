const char* MetadataRetrieverClient::extractMetadata(int keyCode)
{
    ALOGV("extractMetadata");
 Mutex::Autolock lock(mLock);
 if (mRetriever == NULL) {
        ALOGE("retriever is not initialized");
 return NULL;
 }
 return mRetriever->extractMetadata(keyCode);
}
