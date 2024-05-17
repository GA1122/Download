void RenderView::runModal() {
  DCHECK(did_show_) << "should already have shown the view";

  if (RenderThread::current())   
    RenderThread::current()->DoNotSuspendWebKitSharedTimer();

  SendAndRunNestedMessageLoop(new ViewHostMsg_RunModal(routing_id_));
}
