 void UrlData::RedirectTo(const scoped_refptr<UrlData>& url_data) {
  DCHECK(thread_checker_.CalledOnValidThread());
  url_data->multibuffer()->MergeFrom(multibuffer());

  std::vector<RedirectCB> redirect_callbacks;
  redirect_callbacks.swap(redirect_callbacks_);
  for (const RedirectCB& cb : redirect_callbacks) {
    cb.Run(url_data);
  }
}
