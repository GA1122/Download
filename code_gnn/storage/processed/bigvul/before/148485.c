void WebContentsImpl::OnGoToEntryAtOffset(RenderViewHostImpl* source,
                                          int offset) {
  if (!delegate_ || delegate_->OnGoToEntryOffset(offset))
    controller_.GoToOffset(offset);
}
