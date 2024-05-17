void UrlData::Fail() {
  DCHECK(thread_checker_.CalledOnValidThread());
  std::vector<RedirectCB> redirect_callbacks;
  redirect_callbacks.swap(redirect_callbacks_);
  for (const RedirectCB& cb : redirect_callbacks) {
    cb.Run(nullptr);
  }
}
