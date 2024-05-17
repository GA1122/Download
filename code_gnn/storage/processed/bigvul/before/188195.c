  status_t SampleTable::setTimeToSampleParams(
          off64_t data_offset, size_t data_size) {
    if (mTimeToSample != NULL || data_size < 8) {
//     if (!mTimeToSample.empty() || data_size < 8) {
          return ERROR_MALFORMED;
      }
  
  uint8_t header[8];
  if (mDataSource->readAt(
                 data_offset, header, sizeof(header)) < (ssize_t)sizeof(header)) {
  return ERROR_IO;
  }
 
  if (U32_AT(header) != 0) {
  return ERROR_MALFORMED;
 
      }
  
      mTimeToSampleCount = U32_AT(&header[4]);
    uint64_t allocSize = (uint64_t)mTimeToSampleCount * 2 * sizeof(uint32_t);
    if (allocSize > UINT32_MAX) {
//     if ((uint64_t)mTimeToSampleCount >
//         (uint64_t)UINT32_MAX / (2 * sizeof(uint32_t))) {
//          
//          
//          
//          
//          
//          
//         ALOGE("  Error: Time-to-sample table size too large.");
// 
          return ERROR_OUT_OF_RANGE;
      }
    mTimeToSample = new (std::nothrow) uint32_t[mTimeToSampleCount * 2];
    if (!mTimeToSample)
        return ERROR_OUT_OF_RANGE;
  
    size_t size = sizeof(uint32_t) * mTimeToSampleCount * 2;
    if (mDataSource->readAt(
                data_offset + 8, mTimeToSample, size) < (ssize_t)size) {
//      
//      
//     if (!mDataSource->getVector(data_offset + 8, &mTimeToSample,
//                                 mTimeToSampleCount * 2)) {
//         ALOGE("  Error: Incomplete data read for time-to-sample table.");
          return ERROR_IO;
      }
  
    for (uint32_t i = 0; i < mTimeToSampleCount * 2; ++i) {
        mTimeToSample[i] = ntohl(mTimeToSample[i]);
//     for (size_t i = 0; i < mTimeToSample.size(); ++i) {
//         mTimeToSample.editItemAt(i) = ntohl(mTimeToSample[i]);
      }
      return OK;
  }