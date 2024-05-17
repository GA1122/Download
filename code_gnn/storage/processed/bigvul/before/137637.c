scoped_ptr<gfx::GpuMemoryBuffer> RenderThreadImpl::AllocateGpuMemoryBuffer(
    size_t width,
    size_t height,
    unsigned internalformat,
    unsigned usage) {
  TRACE_EVENT0("renderer", "RenderThreadImpl::AllocateGpuMemoryBuffer");

  DCHECK(allocate_gpu_memory_buffer_thread_checker_.CalledOnValidThread());

  if (!GpuMemoryBufferImpl::IsFormatValid(internalformat))
    return scoped_ptr<gfx::GpuMemoryBuffer>();

  gfx::GpuMemoryBufferHandle handle;
  bool success;
  IPC::Message* message = new ChildProcessHostMsg_SyncAllocateGpuMemoryBuffer(
      width, height, internalformat, usage, &handle);

  if (base::MessageLoop::current() == message_loop())
    success = ChildThread::Send(message);
  else
    success = sync_message_filter()->Send(message);

  if (!success)
    return scoped_ptr<gfx::GpuMemoryBuffer>();

  scoped_ptr<GpuMemoryBufferImpl> buffer(GpuMemoryBufferImpl::CreateFromHandle(
      handle,
      gfx::Size(width, height),
      internalformat,
      base::Bind(&DeletedGpuMemoryBuffer,
                 make_scoped_refptr(thread_safe_sender()),
                 handle.type,
                 handle.global_id)));
  if (!buffer) {
    thread_safe_sender()->Send(new ChildProcessHostMsg_DeletedGpuMemoryBuffer(
        handle.type, handle.global_id));
    return scoped_ptr<gfx::GpuMemoryBuffer>();
  }

  return buffer.PassAs<gfx::GpuMemoryBuffer>();
}
