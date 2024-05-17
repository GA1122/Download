void WebPluginDelegateStub::OnPrint(base::SharedMemoryHandle* shared_memory,
                                    size_t* size) {
#if defined(OS_WIN)
  printing::NativeMetafile metafile;
  if (!metafile.CreateDc(NULL, NULL)) {
    NOTREACHED();
    return;
  }
  HDC hdc = metafile.hdc();
  skia::PlatformDevice::InitializeDC(hdc);
  delegate_->Print(hdc);
  if (!metafile.CloseDc()) {
    NOTREACHED();
    return;
  }

  *size = metafile.GetDataSize();
  DCHECK(*size);
  base::SharedMemory shared_buf;
  CreateSharedBuffer(*size, &shared_buf, shared_memory);

  bool success = metafile.GetData(shared_buf.memory(), *size);
  DCHECK(success);
#else
  NOTIMPLEMENTED();
#endif
}
