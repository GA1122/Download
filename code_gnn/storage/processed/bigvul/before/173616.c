status_t NuPlayer::GenericSource::startSources() {
 if (mAudioTrack.mSource != NULL && mAudioTrack.mSource->start() != OK) {
        ALOGE("failed to start audio track!");
 return UNKNOWN_ERROR;
 }

 if (mVideoTrack.mSource != NULL && mVideoTrack.mSource->start() != OK) {
        ALOGE("failed to start video track!");
 return UNKNOWN_ERROR;
 }

 return OK;
}
