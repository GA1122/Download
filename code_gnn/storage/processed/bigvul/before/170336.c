status_t SampleTable::getSampleSize_l(
 uint32_t sampleIndex, size_t *sampleSize) {
 return mSampleIterator->getSampleSizeDirect(
            sampleIndex, sampleSize);
}
