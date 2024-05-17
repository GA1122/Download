status_t MediaPlayer::setMetadataFilter(const Parcel& filter)
{
    ALOGD("setMetadataFilter");
 Mutex::Autolock lock(mLock);
 if (mPlayer == NULL) {
 return NO_INIT;
 }
 return mPlayer->setMetadataFilter(filter);
}
