void ATSParser::Stream::onPayloadData(
 unsigned PTS_DTS_flags, uint64_t PTS, uint64_t  ,
 const uint8_t *data, size_t size, SyncEvent *event) {
#if 0
    ALOGI("payload streamType 0x%02x, PTS = 0x%016llx, dPTS = %lld",
          mStreamType,
          PTS,
 (int64_t)PTS - mPrevPTS);
    mPrevPTS = PTS;
#endif

    ALOGV("onPayloadData mStreamType=0x%02x", mStreamType);

 int64_t timeUs = 0ll;  
 if (PTS_DTS_flags == 2 || PTS_DTS_flags == 3) {
        timeUs = mProgram->convertPTSToTimestamp(PTS);
 }

 status_t err = mQueue->appendData(data, size, timeUs);

 if (mEOSReached) {
        mQueue->signalEOS();
 }

 if (err != OK) {
 return;
 }

    sp<ABuffer> accessUnit;
 bool found = false;
 while ((accessUnit = mQueue->dequeueAccessUnit()) != NULL) {
 if (mSource == NULL) {
            sp<MetaData> meta = mQueue->getFormat();

 if (meta != NULL) {
                ALOGV("Stream PID 0x%08x of type 0x%02x now has data.",
                     mElementaryPID, mStreamType);

 const char *mime;
 if (meta->findCString(kKeyMIMEType, &mime)
 && !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)
 && !IsIDR(accessUnit)) {
 continue;
 }
                mSource = new AnotherPacketSource(meta);
                mSource->queueAccessUnit(accessUnit);
 }
 } else if (mQueue->getFormat() != NULL) {

 if (mSource->getFormat() == NULL) {
                mSource->setFormat(mQueue->getFormat());
 }
            mSource->queueAccessUnit(accessUnit);
 }

 if ((event != NULL) && !found && mQueue->getFormat() != NULL) {
 int32_t sync = 0;
 if (accessUnit->meta()->findInt32("isSync", &sync) && sync) {
 int64_t timeUs;
 if (accessUnit->meta()->findInt64("timeUs", &timeUs)) {
                    found = true;
                    event->init(mPesStartOffset, mSource, timeUs);
 }
 }
 }
 }
}
