status_t MyOggExtractor::seekToTime(int64_t timeUs) {
    timeUs -= mSeekPreRollUs;
 if (timeUs < 0) {
        timeUs = 0;
 }

 if (mTableOfContents.isEmpty()) {
 uint64_t bps = approxBitrate();
 if (bps <= 0) {
 return INVALID_OPERATION;
 }

 off64_t pos = timeUs * bps / 8000000ll;

        ALOGV("seeking to offset %lld", (long long)pos);
 return seekToOffset(pos);
 }

 size_t left = 0;
 size_t right_plus_one = mTableOfContents.size();
 while (left < right_plus_one) {
 size_t center = left + (right_plus_one - left) / 2;

 const TOCEntry &entry = mTableOfContents.itemAt(center);

 if (timeUs < entry.mTimeUs) {
            right_plus_one = center;
 } else if (timeUs > entry.mTimeUs) {
            left = center + 1;
 } else {
            left = center;
 break;
 }
 }

 if (left == mTableOfContents.size()) {
 --left;
 }

 const TOCEntry &entry = mTableOfContents.itemAt(left);

    ALOGV("seeking to entry %zu / %zu at offset %lld",
         left, mTableOfContents.size(), (long long)entry.mPageOffset);

 return seekToOffset(entry.mPageOffset);
}
