void RenderViewImpl::runModal() {
  DCHECK(did_show_) << "should already have shown the view";

  if (RenderThreadImpl::current())   
    RenderThreadImpl::current()->DoNotSuspendWebKitSharedTimer();

  SendAndRunNestedMessageLoop(new ViewHostMsg_RunModal(
      routing_id_, opener_id_));
}
