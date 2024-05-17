sp<MetaData> FLACExtractor::getTrackMetaData(
 size_t index, uint32_t  ) {
 if (mInitCheck != OK || index > 0) {
 return NULL;
 }
 return mTrackMetadata;
}
