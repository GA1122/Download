void SampleTable::buildSampleEntriesTable() {
 Mutex::Autolock autoLock(mLock);

 if (mSampleTimeEntries != NULL) {
 return;
 }

    mSampleTimeEntries = new SampleTimeEntry[mNumSampleSizes];

 uint32_t sampleIndex = 0;
 uint32_t sampleTime = 0;

 for (uint32_t i = 0; i < mTimeToSampleCount; ++i) {
 uint32_t n = mTimeToSample[2 * i];
 uint32_t delta = mTimeToSample[2 * i + 1];

 for (uint32_t j = 0; j < n; ++j) {
 if (sampleIndex < mNumSampleSizes) {

                mSampleTimeEntries[sampleIndex].mSampleIndex = sampleIndex;

 uint32_t compTimeDelta =
                    mCompositionDeltaLookup->getCompositionTimeOffset(
                            sampleIndex);

                mSampleTimeEntries[sampleIndex].mCompositionTime =
                    sampleTime + compTimeDelta;
 }

 ++sampleIndex;
            sampleTime += delta;
 }
 }

    qsort(mSampleTimeEntries, mNumSampleSizes, sizeof(SampleTimeEntry),
 CompareIncreasingTime);
}
