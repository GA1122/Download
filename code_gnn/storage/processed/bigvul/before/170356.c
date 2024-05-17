uint32_t MPEG4Extractor::flags() const {
 return CAN_PAUSE |
 ((mMoofOffset == 0 || mSidxEntries.size() != 0) ?
 (CAN_SEEK_BACKWARD | CAN_SEEK_FORWARD | CAN_SEEK) : 0);
}
