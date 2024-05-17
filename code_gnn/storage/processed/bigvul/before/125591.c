void RenderViewHostImpl::DisownOpener() {
  DCHECK(is_swapped_out_);

  Send(new ViewMsg_DisownOpener(GetRoutingID()));
}
