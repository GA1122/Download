void MetadataRetrieverClient::disconnect()
{
    ALOGV("disconnect from pid %d", mPid);
 Mutex::Autolock lock(mLock);
    mRetriever.clear();
    mThumbnail.clear();
    mAlbumArt.clear();
 IPCThreadState::self()->flushCommands();
}
