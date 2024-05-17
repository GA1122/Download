bool PrintWebViewHelper::CopyMetafileDataToSharedMem(
    PdfMetafileSkia* metafile,
    base::SharedMemoryHandle* shared_mem_handle) {
  uint32 buf_size = metafile->GetDataSize();
  scoped_ptr<base::SharedMemory> shared_buf(
      content::RenderThread::Get()
          ->HostAllocateSharedMemoryBuffer(buf_size)
          .release());

  if (shared_buf) {
    if (shared_buf->Map(buf_size)) {
      metafile->GetData(shared_buf->memory(), buf_size);
      return shared_buf->GiveToProcess(base::GetCurrentProcessHandle(),
                                       shared_mem_handle);
    }
  }
  return false;
}
