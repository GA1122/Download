void WebContentsImpl::SetPageFrozen(bool frozen) {
  DCHECK_NE(Visibility::VISIBLE, GetVisibility());

  SendPageMessage(new PageMsg_SetPageFrozen(MSG_ROUTING_NONE, frozen));
}
