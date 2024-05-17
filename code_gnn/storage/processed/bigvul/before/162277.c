bool CommandBufferProxyImpl::OnMessageReceived(const IPC::Message& message) {
  base::Optional<base::AutoLock> lock;
  if (lock_)
    lock.emplace(*lock_);
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(CommandBufferProxyImpl, message)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_Destroyed, OnDestroyed);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_ConsoleMsg, OnConsoleMessage);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_SignalAck, OnSignalAck);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_SwapBuffersCompleted,
                        OnSwapBuffersCompleted);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_UpdateVSyncParameters,
                        OnUpdateVSyncParameters);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_BufferPresented, OnBufferPresented);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_GetGpuFenceHandleComplete,
                        OnGetGpuFenceHandleComplete);
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (!handled) {
    LOG(ERROR) << "Gpu process sent invalid message.";
    base::AutoLock last_state_lock(last_state_lock_);
    OnGpuAsyncMessageError(gpu::error::kInvalidGpuMessage,
                           gpu::error::kLostContext);
  }
  return handled;
}
