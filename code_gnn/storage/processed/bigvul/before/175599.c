status_t NuMediaExtractor::selectTrack(size_t index) {
 Mutex::Autolock autoLock(mLock);

 if (mImpl == NULL) {
 return -EINVAL;
 }

 if (index >= mImpl->countTracks()) {
 return -ERANGE;
 }

 for (size_t i = 0; i < mSelectedTracks.size(); ++i) {
 TrackInfo *info = &mSelectedTracks.editItemAt(i);

 if (info->mTrackIndex == index) {
 return OK;
 }
 }

    sp<IMediaSource> source = mImpl->getTrack(index);

 status_t ret = source->start();
 if (ret != OK) {
 return ret;
 }

    mSelectedTracks.push();
 TrackInfo *info = &mSelectedTracks.editItemAt(mSelectedTracks.size() - 1);

    info->mSource = source;
    info->mTrackIndex = index;
    info->mFinalResult = OK;
    info->mSample = NULL;
    info->mSampleTimeUs = -1ll;
    info->mTrackFlags = 0;

 const char *mime;
    CHECK(source->getFormat()->findCString(kKeyMIMEType, &mime));

 if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_VORBIS)) {
        info->mTrackFlags |= kIsVorbis;
 }

 return OK;
}
