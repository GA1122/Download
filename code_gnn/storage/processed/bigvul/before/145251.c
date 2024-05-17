void Dispatcher::PopulateSourceMap() {
  const std::vector<std::pair<std::string, int> > resources = GetJsResources();
  for (std::vector<std::pair<std::string, int> >::const_iterator resource =
           resources.begin();
       resource != resources.end();
       ++resource) {
    source_map_.RegisterSource(resource->first, resource->second);
  }
  delegate_->PopulateSourceMap(&source_map_);
}
