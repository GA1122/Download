int16_t AudioSource::getMaxAmplitude() {
 if (!mTrackMaxAmplitude) {
        mTrackMaxAmplitude = true;
 }
 int16_t value = mMaxAmplitude;
    mMaxAmplitude = 0;
    ALOGV("max amplitude since last call: %d", value);
 return value;
}
