bool PrintWebViewHelper::CopyMetafileDataToSharedMem(
    printing::NativeMetafile* metafile,
    base::SharedMemoryHandle* shared_mem_handle) {
  uint32 buf_size = metafile->GetDataSize();
  base::SharedMemoryHandle mem_handle;
  if (Send(new ViewHostMsg_AllocateSharedMemoryBuffer(buf_size, &mem_handle))) {
    if (base::SharedMemory::IsHandleValid(mem_handle)) {
      base::SharedMemory shared_buf(mem_handle, false);
      if (shared_buf.Map(buf_size)) {
        metafile->GetData(shared_buf.memory(), buf_size);
        shared_buf.GiveToProcess(base::GetCurrentProcessHandle(),
                                 shared_mem_handle);
        return true;
      }
    }
  }
  NOTREACHED();
  return false;
}
