UrlData::~UrlData() {
  UMA_HISTOGRAM_MEMORY_KB("Media.BytesReadFromCache",
                          BytesReadFromCache() >> 10);
  UMA_HISTOGRAM_MEMORY_KB("Media.BytesReadFromNetwork",
                          BytesReadFromNetwork() >> 10);
  DCHECK_EQ(0, playing_);
  DCHECK_EQ(0, preloading_);
}
