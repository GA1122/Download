int64_t ATSParser::Program::convertPTSToTimestamp(uint64_t PTS) {
    PTS = recoverPTS(PTS);

 if (!(mParser->mFlags & TS_TIMESTAMPS_ARE_ABSOLUTE)) {
 if (!mFirstPTSValid) {
            mFirstPTSValid = true;
            mFirstPTS = PTS;
            PTS = 0;
 } else if (PTS < mFirstPTS) {
            PTS = 0;
 } else {
            PTS -= mFirstPTS;
 }
 }

 int64_t timeUs = (PTS * 100) / 9;

 if (mParser->mAbsoluteTimeAnchorUs >= 0ll) {
        timeUs += mParser->mAbsoluteTimeAnchorUs;
 }

 if (mParser->mTimeOffsetValid) {
        timeUs += mParser->mTimeOffsetUs;
 }

 return timeUs;
}
