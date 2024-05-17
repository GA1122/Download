size_t GetMaxParallelPreload() {
  if (base::FeatureList::IsEnabled(media::kLimitParallelMediaPreloading))
    return kMaxParallelPreload;
  return std::numeric_limits<size_t>::max();
}
