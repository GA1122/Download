void UrlData::set_cacheable(bool cacheable) {
  DCHECK(thread_checker_.CalledOnValidThread());
  cacheable_ = cacheable;
}
