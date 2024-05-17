int64_t NuPlayer::GenericSource::getLastReadPosition() {
 if (mAudioTrack.mSource != NULL) {
 return mAudioTimeUs;
 } else if (mVideoTrack.mSource != NULL) {
 return mVideoTimeUs;
 } else {
 return 0;
 }
}
