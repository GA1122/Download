bool RenderViewImpl::SendAndRunNestedMessageLoop(IPC::SyncMessage* message) {
  if (RenderThreadImpl::current())   
    RenderThreadImpl::current()->DoNotNotifyWebKitOfModalLoop();

  message->EnableMessagePumping();   
  return Send(message);
}
