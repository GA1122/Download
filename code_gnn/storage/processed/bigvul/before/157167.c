void UrlData::set_etag(const std::string& etag) {
  DCHECK(thread_checker_.CalledOnValidThread());
  etag_ = etag;
}
