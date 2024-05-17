sp<MetaData> OggExtractor::getTrackMetaData(
 size_t index, uint32_t  ) {
 if (index >= 1) {
 return NULL;
 }

 return mImpl->getFormat();
}
