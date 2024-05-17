void ResourceLoader::SetDefersLoading(bool defers) {
  DCHECK(loader_);
  loader_->SetDefersLoading(defers);
}
