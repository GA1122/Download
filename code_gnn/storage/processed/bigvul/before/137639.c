void DeletedGpuMemoryBuffer(ThreadSafeSender* sender,
                            gfx::GpuMemoryBufferType type,
                            const gfx::GpuMemoryBufferId& id) {
  TRACE_EVENT0("renderer", "RenderThreadImpl::DeletedGpuMemoryBuffer");
  sender->Send(new ChildProcessHostMsg_DeletedGpuMemoryBuffer(type, id));
}
