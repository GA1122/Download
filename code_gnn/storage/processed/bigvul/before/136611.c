ResourceTimingInfo* DocumentLoader::GetNavigationTimingInfo() const {
  DCHECK(Fetcher());
  return Fetcher()->GetNavigationTimingInfo();
}
