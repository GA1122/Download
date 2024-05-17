void UrlData::set_range_supported() {
  DCHECK(thread_checker_.CalledOnValidThread());
  range_supported_ = true;
}
