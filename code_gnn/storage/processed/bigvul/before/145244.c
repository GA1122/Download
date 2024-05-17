void Dispatcher::OnSuspend(const std::string& extension_id) {
  DispatchEvent(extension_id, kOnSuspendEvent);
  RenderThread::Get()->Send(new ExtensionHostMsg_SuspendAck(extension_id));
}
