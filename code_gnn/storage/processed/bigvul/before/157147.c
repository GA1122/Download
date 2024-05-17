void UrlData::OnRedirect(const RedirectCB& cb) {
  DCHECK(thread_checker_.CalledOnValidThread());
  redirect_callbacks_.push_back(cb);
}
