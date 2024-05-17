bool TransportTexture::OnMessageReceived(const IPC::Message& msg) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(TransportTexture, msg)
    IPC_MESSAGE_HANDLER(GpuTransportTextureMsg_Destroy,
                        OnDestroy)
    IPC_MESSAGE_HANDLER(GpuTransportTextureMsg_TexturesCreated,
                        OnTexturesCreated)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  DCHECK(handled);
  return handled;
}
