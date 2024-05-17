void WebLocalFrameImpl::SendPings(const WebURL& destination_url) {
  DCHECK(GetFrame());
  DCHECK(context_menu_node_.Get());
  Element* anchor = context_menu_node_->EnclosingLinkEventParentOrSelf();
  if (isHTMLAnchorElement(anchor))
    toHTMLAnchorElement(anchor)->SendPings(destination_url);
}
