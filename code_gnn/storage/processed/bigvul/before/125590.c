void RenderViewHostImpl::DisassociateFromPopupCount() {
  Send(new ViewMsg_DisassociateFromPopupCount(GetRoutingID()));
}
