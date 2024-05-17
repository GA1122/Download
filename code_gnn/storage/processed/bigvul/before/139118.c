void RenderProcessHostImpl::AddFilter(BrowserMessageFilter* filter) {
  filter->RegisterAssociatedInterfaces(channel_.get());
  channel_->AddFilter(filter->GetFilter());
}
