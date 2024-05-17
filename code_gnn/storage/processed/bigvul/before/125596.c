void RenderViewHostImpl::DragTargetDragLeave() {
  Send(new DragMsg_TargetDragLeave(GetRoutingID()));
}
