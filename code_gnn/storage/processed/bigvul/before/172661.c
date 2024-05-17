status_t MediaPlayer::getMetadata(bool update_only, bool apply_filter, Parcel *metadata)
{
    ALOGD("getMetadata");
 Mutex::Autolock lock(mLock);
 if (mPlayer == NULL) {
 return NO_INIT;
 }
 return mPlayer->getMetadata(update_only, apply_filter, metadata);
}
