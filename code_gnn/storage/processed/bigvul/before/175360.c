sp<IMediaSource> OggExtractor::getTrack(size_t index) {
 if (index >= 1) {
 return NULL;
 }

 return new OggSource(this);
}
