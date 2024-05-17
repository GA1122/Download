void MediaMetadataRetriever::disconnect()
{
    ALOGV("disconnect");
    sp<IMediaMetadataRetriever> retriever;
 {
 Mutex::Autolock _l(mLock);
        retriever = mRetriever;
        mRetriever.clear();
 }
 if (retriever != 0) {
        retriever->disconnect();
 }
}
