bool RenderView::SendAndRunNestedMessageLoop(IPC::SyncMessage* message) {
  if (RenderThread::current())   
    RenderThread::current()->DoNotNotifyWebKitOfModalLoop();

  message->EnableMessagePumping();   
  return Send(message);
}
