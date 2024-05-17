void SampleTable::CompositionDeltaLookup::setEntries(
 const int32_t *deltaEntries, size_t numDeltaEntries) {
 Mutex::Autolock autolock(mLock);

    mDeltaEntries = deltaEntries;
    mNumDeltaEntries = numDeltaEntries;
    mCurrentDeltaEntry = 0;
    mCurrentEntrySampleIndex = 0;
}
