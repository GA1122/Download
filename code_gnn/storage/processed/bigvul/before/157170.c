void UrlData::set_valid_until(base::Time valid_until) {
  DCHECK(thread_checker_.CalledOnValidThread());
  valid_until_ = valid_until;
}
