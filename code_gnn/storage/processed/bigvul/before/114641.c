bool WebPluginDelegateProxy::OnMessageReceived(const IPC::Message& msg) {
  content::GetContentClient()->SetActiveURL(page_url_);

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(WebPluginDelegateProxy, msg)
    IPC_MESSAGE_HANDLER(PluginHostMsg_SetWindow, OnSetWindow)
#if defined(OS_WIN)
    IPC_MESSAGE_HANDLER(PluginHostMsg_SetWindowlessPumpEvent,
                        OnSetWindowlessPumpEvent)
    IPC_MESSAGE_HANDLER(PluginHostMsg_NotifyIMEStatus,
                        OnNotifyIMEStatus)
#endif
    IPC_MESSAGE_HANDLER(PluginHostMsg_CancelResource, OnCancelResource)
    IPC_MESSAGE_HANDLER(PluginHostMsg_InvalidateRect, OnInvalidateRect)
    IPC_MESSAGE_HANDLER(PluginHostMsg_GetWindowScriptNPObject,
                        OnGetWindowScriptNPObject)
    IPC_MESSAGE_HANDLER(PluginHostMsg_GetPluginElement,
                        OnGetPluginElement)
    IPC_MESSAGE_HANDLER(PluginHostMsg_ResolveProxy, OnResolveProxy)
    IPC_MESSAGE_HANDLER(PluginHostMsg_SetCookie, OnSetCookie)
    IPC_MESSAGE_HANDLER(PluginHostMsg_GetCookies, OnGetCookies)
    IPC_MESSAGE_HANDLER(PluginHostMsg_URLRequest, OnHandleURLRequest)
    IPC_MESSAGE_HANDLER(PluginHostMsg_CancelDocumentLoad, OnCancelDocumentLoad)
    IPC_MESSAGE_HANDLER(PluginHostMsg_InitiateHTTPRangeRequest,
                        OnInitiateHTTPRangeRequest)
    IPC_MESSAGE_HANDLER(PluginHostMsg_DeferResourceLoading,
                        OnDeferResourceLoading)

#if defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(PluginHostMsg_FocusChanged,
                        OnFocusChanged);
    IPC_MESSAGE_HANDLER(PluginHostMsg_StartIme,
                        OnStartIme);
    IPC_MESSAGE_HANDLER(PluginHostMsg_BindFakePluginWindowHandle,
                        OnBindFakePluginWindowHandle);
    IPC_MESSAGE_HANDLER(PluginHostMsg_AcceleratedSurfaceSetIOSurface,
                        OnAcceleratedSurfaceSetIOSurface)
    IPC_MESSAGE_HANDLER(PluginHostMsg_AcceleratedSurfaceSetTransportDIB,
                        OnAcceleratedSurfaceSetTransportDIB)
    IPC_MESSAGE_HANDLER(PluginHostMsg_AllocTransportDIB,
                        OnAcceleratedSurfaceAllocTransportDIB)
    IPC_MESSAGE_HANDLER(PluginHostMsg_FreeTransportDIB,
                        OnAcceleratedSurfaceFreeTransportDIB)
    IPC_MESSAGE_HANDLER(PluginHostMsg_AcceleratedSurfaceBuffersSwapped,
                        OnAcceleratedSurfaceBuffersSwapped)
    IPC_MESSAGE_HANDLER(PluginHostMsg_AcceleratedPluginEnabledRendering,
                        OnAcceleratedPluginEnabledRendering)
    IPC_MESSAGE_HANDLER(PluginHostMsg_AcceleratedPluginAllocatedIOSurface,
                        OnAcceleratedPluginAllocatedIOSurface)
    IPC_MESSAGE_HANDLER(PluginHostMsg_AcceleratedPluginSwappedIOSurface,
                        OnAcceleratedPluginSwappedIOSurface)
#endif
    IPC_MESSAGE_HANDLER(PluginHostMsg_URLRedirectResponse,
                        OnURLRedirectResponse)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  DCHECK(handled);
  return handled;
}
