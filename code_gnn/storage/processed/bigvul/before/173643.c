ATSParser::ATSParser(uint32_t flags)
 : mFlags(flags),
      mAbsoluteTimeAnchorUs(-1ll),
      mTimeOffsetValid(false),
      mTimeOffsetUs(0ll),
      mLastRecoveredPTS(-1ll),
      mNumTSPacketsParsed(0),
      mNumPCRs(0) {
    mPSISections.add(0  , new PSISection);
}
