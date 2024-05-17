void UrlData::set_last_modified(base::Time last_modified) {
  DCHECK(thread_checker_.CalledOnValidThread());
  last_modified_ = last_modified;
}
