void RenderThread::OnControlMessageReceived(const IPC::Message& msg) {
  if (appcache_dispatcher_->OnMessageReceived(msg))
    return;
  if (socket_stream_dispatcher_->OnMessageReceived(msg))
    return;

  IPC_BEGIN_MESSAGE_MAP(RenderThread, msg)
    IPC_MESSAGE_HANDLER(ViewMsg_VisitedLink_NewTable, OnUpdateVisitedLinks)
    IPC_MESSAGE_HANDLER(ViewMsg_VisitedLink_Add, OnAddVisitedLinks)
    IPC_MESSAGE_HANDLER(ViewMsg_VisitedLink_Reset, OnResetVisitedLinks)
    IPC_MESSAGE_HANDLER(ViewMsg_SetNextPageID, OnSetNextPageID)
    IPC_MESSAGE_HANDLER(ViewMsg_SetCSSColors, OnSetCSSColors)
    IPC_MESSAGE_HANDLER(ViewMsg_New, OnCreateNewView)
    IPC_MESSAGE_HANDLER(ViewMsg_SetCacheCapacities, OnSetCacheCapacities)
    IPC_MESSAGE_HANDLER(ViewMsg_GetRendererHistograms,
                        OnGetRendererHistograms)
#if defined(USE_TCMALLOC)
    IPC_MESSAGE_HANDLER(ViewMsg_GetRendererTcmalloc,
                        OnGetRendererTcmalloc)
#endif
    IPC_MESSAGE_HANDLER(ViewMsg_GetCacheResourceStats,
                        OnGetCacheResourceStats)
    IPC_MESSAGE_HANDLER(ViewMsg_UserScripts_UpdatedScripts,
                        OnUpdateUserScripts)
    IPC_MESSAGE_HANDLER(ViewMsg_ExtensionMessageInvoke,
                        OnExtensionMessageInvoke)
    IPC_MESSAGE_HANDLER(ViewMsg_Extension_SetFunctionNames,
                        OnSetExtensionFunctionNames)
    IPC_MESSAGE_HANDLER(ViewMsg_PurgeMemory, OnPurgeMemory)
    IPC_MESSAGE_HANDLER(ViewMsg_PurgePluginListCache,
                        OnPurgePluginListCache)
    IPC_MESSAGE_HANDLER(ViewMsg_Extension_UpdatePageActions,
                        OnPageActionsUpdated)
    IPC_MESSAGE_HANDLER(ViewMsg_Extension_SetAPIPermissions,
                        OnExtensionSetAPIPermissions)
    IPC_MESSAGE_HANDLER(ViewMsg_Extension_SetHostPermissions,
                        OnExtensionSetHostPermissions)
    IPC_MESSAGE_HANDLER(ViewMsg_DOMStorageEvent,
                        OnDOMStorageEvent)
    IPC_MESSAGE_HANDLER(ViewMsg_Extension_SetL10nMessages,
                        OnExtensionSetL10nMessages)
#if defined(IPC_MESSAGE_LOG_ENABLED)
    IPC_MESSAGE_HANDLER(ViewMsg_SetIPCLoggingEnabled,
                        OnSetIPCLoggingEnabled)
#endif
  IPC_END_MESSAGE_MAP()
}
