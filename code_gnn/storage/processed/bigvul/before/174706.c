sp<IMemory> MediaMetadataRetriever::extractAlbumArt()
{
    ALOGV("extractAlbumArt");
 Mutex::Autolock _l(mLock);
 if (mRetriever == 0) {
        ALOGE("retriever is not initialized");
 return NULL;
 }
 return mRetriever->extractAlbumArt();
}
