void UrlData::OnEmpty() {
  DCHECK(thread_checker_.CalledOnValidThread());
  url_index_->RemoveUrlData(this);
}
