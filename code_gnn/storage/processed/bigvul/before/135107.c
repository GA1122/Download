bool AppCacheDispatcherHost::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(AppCacheDispatcherHost, message)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_RegisterHost, OnRegisterHost)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_UnregisterHost, OnUnregisterHost)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_SetSpawningHostId, OnSetSpawningHostId)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_GetResourceList, OnGetResourceList)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_SelectCache, OnSelectCache)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_SelectCacheForWorker,
                        OnSelectCacheForWorker)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_SelectCacheForSharedWorker,
                        OnSelectCacheForSharedWorker)
    IPC_MESSAGE_HANDLER(AppCacheHostMsg_MarkAsForeignEntry,
                        OnMarkAsForeignEntry)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(AppCacheHostMsg_GetStatus, OnGetStatus)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(AppCacheHostMsg_StartUpdate, OnStartUpdate)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(AppCacheHostMsg_SwapCache, OnSwapCache)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
