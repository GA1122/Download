void ResourceFetcher::SetDefersLoading(bool defers) {
  for (const auto& loader : non_blocking_loaders_)
    loader->SetDefersLoading(defers);
  for (const auto& loader : loaders_)
    loader->SetDefersLoading(defers);
}
