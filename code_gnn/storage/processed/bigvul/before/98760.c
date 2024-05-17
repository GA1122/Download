bool WebPluginDelegateProxy::CreateSharedBitmap(
    scoped_ptr<TransportDIB>* memory,
    scoped_ptr<skia::PlatformCanvas>* canvas) {
  const size_t size = BitmapSizeForPluginRect(plugin_rect_);
#if defined(OS_POSIX) && !defined(OS_MACOSX)
  memory->reset(TransportDIB::Create(size, 0));
  if (!memory->get())
    return false;
#endif
#if defined(OS_MACOSX)
  TransportDIB::Handle handle;
  IPC::Message* msg = new ViewHostMsg_AllocTransportDIB(size, true, &handle);
  if (!RenderThread::current()->Send(msg))
    return false;
  if (handle.fd < 0)
    return false;
  memory->reset(TransportDIB::Map(handle));
#else
  static uint32 sequence_number = 0;
  memory->reset(TransportDIB::Create(size, sequence_number++));
#endif
  canvas->reset((*memory)->GetPlatformCanvas(plugin_rect_.width(),
                                             plugin_rect_.height()));
  return !!canvas->get();
}
