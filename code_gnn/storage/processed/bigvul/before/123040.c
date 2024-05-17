void RenderWidgetHostImpl::Replace(const string16& word) {
  Send(new ViewMsg_Replace(routing_id_, word));
}
