bool VBRISeeker::getOffsetForTime(int64_t *timeUs, off64_t *pos) {
 if (mDurationUs < 0 || mSegments.size() == 0) {
 return false;
 }

 int64_t segmentDurationUs = mDurationUs / mSegments.size();

 int64_t nowUs = 0;
 *pos = mBasePos;
 size_t segmentIndex = 0;
 while (segmentIndex < mSegments.size() && nowUs < *timeUs) {
        nowUs += segmentDurationUs;
 *pos += mSegments.itemAt(segmentIndex++);
 }

    ALOGV("getOffsetForTime %lld us => 0x%016llx", (long long)*timeUs, (long long)*pos);

 *timeUs = nowUs;

 return true;
}
