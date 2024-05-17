status_t NuPlayer::GenericSource::dequeueAccessUnit(
 bool audio, sp<ABuffer> *accessUnit) {
 Track *track = audio ? &mAudioTrack : &mVideoTrack;

 if (track->mSource == NULL) {
 return -EWOULDBLOCK;
 }

 if (mIsWidevine && !audio) {
        postReadBuffer(MEDIA_TRACK_TYPE_VIDEO);
 }

 status_t finalResult;
 if (!track->mPackets->hasBufferAvailable(&finalResult)) {
 return (finalResult == OK ? -EWOULDBLOCK : finalResult);
 }

 status_t result = track->mPackets->dequeueAccessUnit(accessUnit);

 if (!track->mPackets->hasBufferAvailable(&finalResult)) {
        postReadBuffer(audio? MEDIA_TRACK_TYPE_AUDIO : MEDIA_TRACK_TYPE_VIDEO);
 }

 if (result != OK) {
 if (mSubtitleTrack.mSource != NULL) {
            mSubtitleTrack.mPackets->clear();
            mFetchSubtitleDataGeneration++;
 }
 if (mTimedTextTrack.mSource != NULL) {
            mTimedTextTrack.mPackets->clear();
            mFetchTimedTextDataGeneration++;
 }
 return result;
 }

 int64_t timeUs;
 status_t eosResult;  
    CHECK((*accessUnit)->meta()->findInt64("timeUs", &timeUs));

 if (mSubtitleTrack.mSource != NULL
 && !mSubtitleTrack.mPackets->hasBufferAvailable(&eosResult)) {
        sp<AMessage> msg = new AMessage(kWhatFetchSubtitleData, id());
        msg->setInt64("timeUs", timeUs);
        msg->setInt32("generation", mFetchSubtitleDataGeneration);
        msg->post();
 }

 if (mTimedTextTrack.mSource != NULL
 && !mTimedTextTrack.mPackets->hasBufferAvailable(&eosResult)) {
        sp<AMessage> msg = new AMessage(kWhatFetchTimedTextData, id());
        msg->setInt64("timeUs", timeUs);
        msg->setInt32("generation", mFetchTimedTextDataGeneration);
        msg->post();
 }

 return result;
}
