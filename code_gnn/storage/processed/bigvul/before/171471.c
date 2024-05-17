bool ASessionDescription::setTo(const void *data, size_t size) {
    mIsValid = parse(data, size);

 if (!mIsValid) {
        mTracks.clear();
        mFormats.clear();
 }

 return mIsValid;
}
