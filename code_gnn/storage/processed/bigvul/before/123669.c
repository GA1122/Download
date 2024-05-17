bool GpuCommandBufferStub::OnMessageReceived(const IPC::Message& message) {
  FastSetActiveURL(active_url_, active_url_hash_);

  if (decoder_.get() &&
      message.type() != GpuCommandBufferMsg_Echo::ID &&
      message.type() != GpuCommandBufferMsg_RetireSyncPoint::ID &&
      message.type() != GpuCommandBufferMsg_WaitSyncPoint::ID) {
    if (!MakeCurrent())
      return false;
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(GpuCommandBufferStub, message)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_Initialize,
                                    OnInitialize);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_SetGetBuffer,
                                    OnSetGetBuffer);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_SetSharedStateBuffer,
                                    OnSetSharedStateBuffer);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_SetParent,
                                    OnSetParent);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_Echo, OnEcho);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_GetState, OnGetState);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_GetStateFast,
                                    OnGetStateFast);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_AsyncFlush, OnAsyncFlush);
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_Rescheduled, OnRescheduled);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_CreateTransferBuffer,
                                    OnCreateTransferBuffer);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_RegisterTransferBuffer,
                                    OnRegisterTransferBuffer);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_DestroyTransferBuffer,
                                    OnDestroyTransferBuffer);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_GetTransferBuffer,
                                    OnGetTransferBuffer);
    IPC_MESSAGE_HANDLER_DELAY_REPLY(GpuCommandBufferMsg_CreateVideoDecoder,
                                    OnCreateVideoDecoder)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_DestroyVideoDecoder,
                        OnDestroyVideoDecoder)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_SetSurfaceVisible,
                        OnSetSurfaceVisible)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_DiscardBackbuffer,
                        OnDiscardBackbuffer)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_EnsureBackbuffer,
                        OnEnsureBackbuffer)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_RetireSyncPoint,
                        OnRetireSyncPoint)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_WaitSyncPoint,
                        OnWaitSyncPoint)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_SignalSyncPoint,
                        OnSignalSyncPoint)
    IPC_MESSAGE_HANDLER(GpuCommandBufferMsg_SendClientManagedMemoryStats,
                        OnReceivedClientManagedMemoryStats)
    IPC_MESSAGE_HANDLER(
        GpuCommandBufferMsg_SetClientHasMemoryAllocationChangedCallback,
        OnSetClientHasMemoryAllocationChangedCallback)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  ScheduleDelayedWork(kHandleMoreWorkPeriodMs);

  DCHECK(handled);
  return handled;
}
