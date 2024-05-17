ATSParser::Stream::Stream(
 Program *program,
 unsigned elementaryPID,
 unsigned streamType,
 unsigned PCR_PID)
 : mProgram(program),
      mElementaryPID(elementaryPID),
      mStreamType(streamType),
      mPCR_PID(PCR_PID),
      mExpectedContinuityCounter(-1),
      mPayloadStarted(false),
      mEOSReached(false),
      mPrevPTS(0),
      mQueue(NULL) {
 switch (mStreamType) {
 case STREAMTYPE_H264:
            mQueue = new ElementaryStreamQueue(
 ElementaryStreamQueue::H264,
 (mProgram->parserFlags() & ALIGNED_VIDEO_DATA)
 ? ElementaryStreamQueue::kFlag_AlignedData : 0);
 break;
 case STREAMTYPE_MPEG2_AUDIO_ADTS:
            mQueue = new ElementaryStreamQueue(ElementaryStreamQueue::AAC);
 break;
 case STREAMTYPE_MPEG1_AUDIO:
 case STREAMTYPE_MPEG2_AUDIO:
            mQueue = new ElementaryStreamQueue(
 ElementaryStreamQueue::MPEG_AUDIO);
 break;

 case STREAMTYPE_MPEG1_VIDEO:
 case STREAMTYPE_MPEG2_VIDEO:
            mQueue = new ElementaryStreamQueue(
 ElementaryStreamQueue::MPEG_VIDEO);
 break;

 case STREAMTYPE_MPEG4_VIDEO:
            mQueue = new ElementaryStreamQueue(
 ElementaryStreamQueue::MPEG4_VIDEO);
 break;

 case STREAMTYPE_LPCM_AC3:
 case STREAMTYPE_AC3:
            mQueue = new ElementaryStreamQueue(
 ElementaryStreamQueue::AC3);
 break;

 case STREAMTYPE_METADATA:
            mQueue = new ElementaryStreamQueue(
 ElementaryStreamQueue::METADATA);
 break;

 default:
 break;
 }

    ALOGV("new stream PID 0x%02x, type 0x%02x", elementaryPID, streamType);

 if (mQueue != NULL) {
        mBuffer = new ABuffer(192 * 1024);
        mBuffer->setRange(0, 0);
 }
}
