uint64_t MyVorbisExtractor::approxBitrate() const {
 if (mVi.bitrate_nominal != 0) {
 return mVi.bitrate_nominal;
 }

 return (mVi.bitrate_lower + mVi.bitrate_upper) / 2;
}
