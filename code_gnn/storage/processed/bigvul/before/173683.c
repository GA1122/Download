void ATSParser::Stream::signalDiscontinuity(
 DiscontinuityType type, const sp<AMessage> &extra) {
    mExpectedContinuityCounter = -1;

 if (mQueue == NULL) {
 return;
 }

    mPayloadStarted = false;
    mEOSReached = false;
    mBuffer->setRange(0, 0);

 bool clearFormat = false;
 if (isAudio()) {
 if (type & DISCONTINUITY_AUDIO_FORMAT) {
            clearFormat = true;
 }
 } else {
 if (type & DISCONTINUITY_VIDEO_FORMAT) {
            clearFormat = true;
 }
 }

    mQueue->clear(clearFormat);

 if (type & DISCONTINUITY_TIME) {
 uint64_t resumeAtPTS;
 if (extra != NULL
 && extra->findInt64(
 IStreamListener::kKeyResumeAtPTS,
 (int64_t *)&resumeAtPTS)) {
 int64_t resumeAtMediaTimeUs =
                mProgram->convertPTSToTimestamp(resumeAtPTS);

            extra->setInt64("resume-at-mediaTimeUs", resumeAtMediaTimeUs);
 }
 }

 if (mSource != NULL) {
        mSource->queueDiscontinuity(type, extra, true);
 }
}
