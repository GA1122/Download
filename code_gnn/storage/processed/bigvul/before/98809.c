void WebPluginDelegateProxy::Print(gfx::NativeDrawingContext context) {
  base::SharedMemoryHandle shared_memory;
  uint32 size;
  if (!Send(new PluginMsg_Print(instance_id_, &shared_memory, &size)))
    return;

  base::SharedMemory memory(shared_memory, true);
  if (!memory.Map(size)) {
    NOTREACHED();
    return;
  }

#if defined(OS_WIN)
  printing::NativeMetafile metafile;
  if (!metafile.CreateFromData(memory.memory(), size)) {
    NOTREACHED();
    return;
  }
  metafile.Playback(context, NULL);
#else
  NOTIMPLEMENTED();
#endif
}
