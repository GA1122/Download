status_t SoftAMRNBEncoder::initEncoder() {
 if (AMREncodeInit(&mEncState, &mSidState, false  ) != 0) {
 return UNKNOWN_ERROR;
 }

 return OK;
}
