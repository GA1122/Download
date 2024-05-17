void RenderWidgetHostImpl::Focus() {
  is_focused_ = true;

  Send(new InputMsg_SetFocus(routing_id_, true));

  if (RenderViewHost::From(this) && delegate_)
    delegate_->ReplicatePageFocus(true);
}
