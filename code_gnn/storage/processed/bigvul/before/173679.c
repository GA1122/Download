int64_t ATSParser::Program::recoverPTS(uint64_t PTS_33bit) {
 if (mLastRecoveredPTS < 0ll) {
        mLastRecoveredPTS = static_cast<int64_t>(PTS_33bit);
 } else {
        mLastRecoveredPTS = static_cast<int64_t>(
 ((mLastRecoveredPTS - PTS_33bit + 0x100000000ll)
 & 0xfffffffe00000000ull) | PTS_33bit);
 if (mLastRecoveredPTS < 0ll) {
            ALOGI("Clamping negative recovered PTS (%" PRId64 ") to 0", mLastRecoveredPTS);
            mLastRecoveredPTS = 0ll;
 }
 }

 return mLastRecoveredPTS;
}
