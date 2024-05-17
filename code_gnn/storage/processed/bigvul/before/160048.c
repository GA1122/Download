std::string BackendImpl::HistogramName(const char* name, int experiment) const {
  if (!experiment)
    return base::StringPrintf("DiskCache.%d.%s", cache_type_, name);
  return base::StringPrintf("DiskCache.%d.%s_%d", cache_type_,
                            name, experiment);
}
