void NuPlayer::GenericSource::onMessageReceived(const sp<AMessage> &msg) {
 switch (msg->what()) {
 case kWhatPrepareAsync:
 {
          onPrepareAsync();
 break;
 }
 case kWhatFetchSubtitleData:
 {
          fetchTextData(kWhatSendSubtitleData, MEDIA_TRACK_TYPE_SUBTITLE,
                  mFetchSubtitleDataGeneration, mSubtitleTrack.mPackets, msg);
 break;
 }

 case kWhatFetchTimedTextData:
 {
          fetchTextData(kWhatSendTimedTextData, MEDIA_TRACK_TYPE_TIMEDTEXT,
                  mFetchTimedTextDataGeneration, mTimedTextTrack.mPackets, msg);
 break;
 }

 case kWhatSendSubtitleData:
 {
          sendTextData(kWhatSubtitleData, MEDIA_TRACK_TYPE_SUBTITLE,
                  mFetchSubtitleDataGeneration, mSubtitleTrack.mPackets, msg);
 break;
 }

 case kWhatSendTimedTextData:
 {
          sendTextData(kWhatTimedTextData, MEDIA_TRACK_TYPE_TIMEDTEXT,
                  mFetchTimedTextDataGeneration, mTimedTextTrack.mPackets, msg);
 break;
 }

 case kWhatChangeAVSource:
 {
 int32_t trackIndex;
          CHECK(msg->findInt32("trackIndex", &trackIndex));
 const sp<MediaSource> source = mSources.itemAt(trackIndex);

 Track* track;
 const char *mime;
          media_track_type trackType, counterpartType;
          sp<MetaData> meta = source->getFormat();
          meta->findCString(kKeyMIMEType, &mime);
 if (!strncasecmp(mime, "audio/", 6)) {
              track = &mAudioTrack;
              trackType = MEDIA_TRACK_TYPE_AUDIO;
              counterpartType = MEDIA_TRACK_TYPE_VIDEO;;
 } else {
              CHECK(!strncasecmp(mime, "video/", 6));
              track = &mVideoTrack;
              trackType = MEDIA_TRACK_TYPE_VIDEO;
              counterpartType = MEDIA_TRACK_TYPE_AUDIO;;
 }


 if (track->mSource != NULL) {
              track->mSource->stop();
 }
          track->mSource = source;
          track->mSource->start();
          track->mIndex = trackIndex;

 int64_t timeUs, actualTimeUs;
 const bool formatChange = true;
 if (trackType == MEDIA_TRACK_TYPE_AUDIO) {
              timeUs = mAudioLastDequeueTimeUs;
 } else {
              timeUs = mVideoLastDequeueTimeUs;
 }
          readBuffer(trackType, timeUs, &actualTimeUs, formatChange);
          readBuffer(counterpartType, -1, NULL, formatChange);
          ALOGV("timeUs %lld actualTimeUs %lld", (long long)timeUs, (long long)actualTimeUs);

 break;
 }

 case kWhatStart:
 case kWhatResume:
 {
          restartPollBuffering();
 break;
 }

 case kWhatPollBuffering:
 {
 int32_t generation;
          CHECK(msg->findInt32("generation", &generation));
 if (generation == mPollBufferingGeneration) {
              onPollBuffering();
 }
 break;
 }

 case kWhatGetFormat:
 {
          onGetFormatMeta(msg);
 break;
 }

 case kWhatGetSelectedTrack:
 {
          onGetSelectedTrack(msg);
 break;
 }

 case kWhatSelectTrack:
 {
          onSelectTrack(msg);
 break;
 }

 case kWhatSeek:
 {
          onSeek(msg);
 break;
 }

 case kWhatReadBuffer:
 {
          onReadBuffer(msg);
 break;
 }

 case kWhatSecureDecodersInstantiated:
 {
 int32_t err;
          CHECK(msg->findInt32("err", &err));
          onSecureDecodersInstantiated(err);
 break;
 }

 case kWhatStopWidevine:
 {
          mStopRead = true;
 if (mVideoTrack.mSource != NULL) {
              mVideoTrack.mPackets->clear();
 }
          sp<AMessage> response = new AMessage;
          sp<AReplyToken> replyID;
          CHECK(msg->senderAwaitsResponse(&replyID));
          response->postReply(replyID);
 break;
 }
 default:
 Source::onMessageReceived(msg);
 break;
 }
}