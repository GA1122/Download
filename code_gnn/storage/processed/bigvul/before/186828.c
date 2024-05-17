 bool PrintRenderFrameHelper::CopyMetafileDataToSharedMem(
// bool PrintRenderFrameHelper::CopyMetafileDataToReadOnlySharedMem(
      const PdfMetafileSkia& metafile,
      base::SharedMemoryHandle* shared_mem_handle) {
    uint32_t buf_size = metafile.GetDataSize();
    if (buf_size == 0)
      return false;
  
  std::unique_ptr<base::SharedMemory> shared_buf(
      content::RenderThread::Get()->HostAllocateSharedMemoryBuffer(buf_size));
  if (!shared_buf)
//   mojo::ScopedSharedBufferHandle buffer =
//       mojo::SharedBufferHandle::Create(buf_size);
//   if (!buffer.is_valid())
      return false;
  
  if (!shared_buf->Map(buf_size))
//   mojo::ScopedSharedBufferMapping mapping = buffer->Map(buf_size);
//   if (!mapping)
      return false;
  
  if (!metafile.GetData(shared_buf->memory(), buf_size))
//   if (!metafile.GetData(mapping.get(), buf_size))
      return false;
  
  *shared_mem_handle =
      base::SharedMemory::DuplicateHandle(shared_buf->handle());
//   MojoResult result = mojo::UnwrapSharedMemoryHandle(
//       buffer->Clone(mojo::SharedBufferHandle::AccessMode::READ_ONLY),
//       shared_mem_handle, nullptr, nullptr);
//   DCHECK_EQ(MOJO_RESULT_OK, result);
    return true;
  }