sp<IMediaSource> FLACExtractor::getTrack(size_t index)
{
 if (mInitCheck != OK || index > 0) {
 return NULL;
 }
 return new FLACSource(mDataSource, mTrackMetadata);
}
