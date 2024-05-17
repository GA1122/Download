std::pair<GURL, UrlData::CORSMode> UrlData::key() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return std::make_pair(url(), cors_mode());
}
