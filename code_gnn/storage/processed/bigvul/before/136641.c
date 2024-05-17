DocumentLoader::~DocumentLoader() {
  DCHECK(!frame_);
  DCHECK(!GetResource());
  DCHECK(!application_cache_host_);
  DCHECK_EQ(state_, kSentDidFinishLoad);
}
