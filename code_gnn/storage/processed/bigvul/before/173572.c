sp<MetaData> NuPlayer::GenericSource::doGetFormatMeta(bool audio) const {
    sp<MediaSource> source = audio ? mAudioTrack.mSource : mVideoTrack.mSource;

 if (source == NULL) {
 return NULL;
 }

 return source->getFormat();
}
