base::WaitableEvent* PluginDataRemoverImpl::StartRemoving(
    base::Time begin_time) {
  DCHECK(!context_.get());
  context_ = new Context(begin_time, browser_context_);
  context_->Init(mime_type_);
  return context_->event();
}
