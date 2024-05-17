sp<MetaData> AMRExtractor::getTrackMetaData(size_t index, uint32_t  ) {
 if (mInitCheck != OK || index != 0) {
 return NULL;
 }

 return mMeta;
}
