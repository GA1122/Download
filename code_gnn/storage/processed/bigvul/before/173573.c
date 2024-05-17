ssize_t NuPlayer::GenericSource::doGetSelectedTrack(media_track_type type) const {
 const Track *track = NULL;
 switch (type) {
 case MEDIA_TRACK_TYPE_VIDEO:
        track = &mVideoTrack;
 break;
 case MEDIA_TRACK_TYPE_AUDIO:
        track = &mAudioTrack;
 break;
 case MEDIA_TRACK_TYPE_TIMEDTEXT:
        track = &mTimedTextTrack;
 break;
 case MEDIA_TRACK_TYPE_SUBTITLE:
        track = &mSubtitleTrack;
 break;
 default:
 break;
 }

 if (track != NULL && track->mSource != NULL) {
 return track->mIndex;
 }

 return -1;
}
