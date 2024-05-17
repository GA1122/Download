DocumentLoader::~DocumentLoader() {
  DCHECK(!frame_);
  DCHECK(!main_resource_);
  DCHECK(!application_cache_host_);
  DCHECK_EQ(state_, kSentDidFinishLoad);
}
