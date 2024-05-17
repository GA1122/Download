 GDataRootDirectory::~GDataRootDirectory() {
  STLDeleteValues(&cache_map_);
  cache_map_.clear();

  resource_map_.clear();
}
