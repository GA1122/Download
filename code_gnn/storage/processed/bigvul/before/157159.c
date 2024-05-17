void UrlData::Use() {
  DCHECK(thread_checker_.CalledOnValidThread());
  last_used_ = base::Time::Now();
}
