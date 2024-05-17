SoftAMRNBEncoder::~SoftAMRNBEncoder() {
 if (mEncState != NULL) {
 AMREncodeExit(&mEncState, &mSidState);
        mEncState = mSidState = NULL;
 }
}
