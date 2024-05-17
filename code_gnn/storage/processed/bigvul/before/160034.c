void BackendImpl::FlushIndex() {
  if (index_.get() && !disabled_)
    index_->Flush();
}
