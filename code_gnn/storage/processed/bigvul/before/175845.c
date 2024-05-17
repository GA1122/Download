bool VBRISeeker::getDuration(int64_t *durationUs) {
 if (mDurationUs < 0) {
 return false;
 }

 *durationUs = mDurationUs;

 return true;
}
