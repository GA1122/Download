bool UrlData::IsPreloading() const {
  return preloading_ > 0 && playing_ == 0;
}
