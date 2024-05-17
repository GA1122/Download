bool WebPluginDelegateProxy::CreateBitmap(
    scoped_ptr<TransportDIB>* memory,
    scoped_ptr<skia::PlatformCanvas>* canvas) {
  int width = plugin_rect_.width();
  int height = plugin_rect_.height();
  const size_t stride = skia::PlatformCanvas::StrideForWidth(width);
  const size_t size = stride * height;
#if defined(OS_LINUX)
  memory->reset(TransportDIB::Create(size, 0));
  if (!memory->get())
    return false;
#endif
#if defined(OS_MACOSX)
  TransportDIB::Handle handle;
  IPC::Message* msg = new ViewHostMsg_AllocTransportDIB(size, &handle);
  if (!RenderThread::current()->Send(msg))
    return false;
  if (handle.fd < 0)
    return false;
  memory->reset(TransportDIB::Map(handle));
#else
  static uint32 sequence_number = 0;
  memory->reset(TransportDIB::Create(size, sequence_number++));
#endif
  canvas->reset((*memory)->GetPlatformCanvas(width, height));
  return true;
}
