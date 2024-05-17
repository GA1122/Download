SampleIterator::SampleIterator(SampleTable *table)
 : mTable(table),
      mInitialized(false),
      mTimeToSampleIndex(0),
      mTTSSampleIndex(0),
      mTTSSampleTime(0),
      mTTSCount(0),
      mTTSDuration(0) {
    reset();
}
