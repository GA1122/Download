int64_t OMXCodec::getDecodingTimeUs() {
    CHECK(mIsEncoder && mIsVideo);

 if (mDecodingTimeList.empty()) {
        CHECK(mSignalledEOS || mNoMoreOutputData);
 return 0;
 }

 List<int64_t>::iterator it = mDecodingTimeList.begin();
 int64_t timeUs = *it;
    mDecodingTimeList.erase(it);
 return timeUs;
}
