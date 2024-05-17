status_t NuPlayer::GenericSource::doSelectTrack(size_t trackIndex, bool select, int64_t timeUs) {
 if (trackIndex >= mSources.size()) {
 return BAD_INDEX;
 }

 if (!select) {
 Track* track = NULL;
 if (mSubtitleTrack.mSource != NULL && trackIndex == mSubtitleTrack.mIndex) {
            track = &mSubtitleTrack;
            mFetchSubtitleDataGeneration++;
 } else if (mTimedTextTrack.mSource != NULL && trackIndex == mTimedTextTrack.mIndex) {
            track = &mTimedTextTrack;
            mFetchTimedTextDataGeneration++;
 }
 if (track == NULL) {
 return INVALID_OPERATION;
 }
        track->mSource->stop();
        track->mSource = NULL;
        track->mPackets->clear();
 return OK;
 }

 const sp<MediaSource> source = mSources.itemAt(trackIndex);
    sp<MetaData> meta = source->getFormat();
 const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));
 if (!strncasecmp(mime, "text/", 5)) {
 bool isSubtitle = strcasecmp(mime, MEDIA_MIMETYPE_TEXT_3GPP);
 Track *track = isSubtitle ? &mSubtitleTrack : &mTimedTextTrack;
 if (track->mSource != NULL && track->mIndex == trackIndex) {
 return OK;
 }
        track->mIndex = trackIndex;
 if (track->mSource != NULL) {
            track->mSource->stop();
 }
        track->mSource = mSources.itemAt(trackIndex);
        track->mSource->start();
 if (track->mPackets == NULL) {
            track->mPackets = new AnotherPacketSource(track->mSource->getFormat());
 } else {
            track->mPackets->clear();
            track->mPackets->setFormat(track->mSource->getFormat());

 }

 if (isSubtitle) {
            mFetchSubtitleDataGeneration++;
 } else {
            mFetchTimedTextDataGeneration++;
 }

 status_t eosResult;  
 if (mSubtitleTrack.mSource != NULL
 && !mSubtitleTrack.mPackets->hasBufferAvailable(&eosResult)) {
            sp<AMessage> msg = new AMessage(kWhatFetchSubtitleData, this);
            msg->setInt64("timeUs", timeUs);
            msg->setInt32("generation", mFetchSubtitleDataGeneration);
            msg->post();
 }

 if (mTimedTextTrack.mSource != NULL
 && !mTimedTextTrack.mPackets->hasBufferAvailable(&eosResult)) {
            sp<AMessage> msg = new AMessage(kWhatFetchTimedTextData, this);
            msg->setInt64("timeUs", timeUs);
            msg->setInt32("generation", mFetchTimedTextDataGeneration);
            msg->post();
 }

 return OK;
 } else if (!strncasecmp(mime, "audio/", 6) || !strncasecmp(mime, "video/", 6)) {
 bool audio = !strncasecmp(mime, "audio/", 6);
 Track *track = audio ? &mAudioTrack : &mVideoTrack;
 if (track->mSource != NULL && track->mIndex == trackIndex) {
 return OK;
 }

        sp<AMessage> msg = new AMessage(kWhatChangeAVSource, this);
        msg->setInt32("trackIndex", trackIndex);
        msg->post();
 return OK;
 }

 return INVALID_OPERATION;
}
