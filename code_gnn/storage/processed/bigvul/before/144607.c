bool WebContentsImpl::OnMessageReceived(RenderViewHost* render_view_host,
                                        RenderFrameHost* render_frame_host,
                                        const IPC::Message& message) {
  DCHECK(render_view_host || render_frame_host);
  if (GetWebUI() &&
      static_cast<WebUIImpl*>(GetWebUI())->OnMessageReceived(message)) {
    return true;
  }

  base::ObserverListBase<WebContentsObserver>::Iterator it(&observers_);
  WebContentsObserver* observer;
  if (render_frame_host) {
    while ((observer = it.GetNext()) != NULL)
      if (observer->OnMessageReceived(message, render_frame_host))
        return true;
  } else {
    while ((observer = it.GetNext()) != NULL)
      if (observer->OnMessageReceived(message))
        return true;
  }

  if (render_frame_host)
    render_frame_message_source_ = render_frame_host;
  else
    render_view_message_source_ = render_view_host;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(WebContentsImpl, message)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DomOperationResponse,
                        OnDomOperationResponse)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidChangeThemeColor,
                        OnThemeColorChanged)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidFinishDocumentLoad,
                        OnDocumentLoadedInFrame)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidFinishLoad, OnDidFinishLoad)
    IPC_MESSAGE_HANDLER(FrameHostMsg_OpenColorChooser, OnOpenColorChooser)
    IPC_MESSAGE_HANDLER(FrameHostMsg_EndColorChooser, OnEndColorChooser)
    IPC_MESSAGE_HANDLER(FrameHostMsg_SetSelectedColorInColorChooser,
                        OnSetSelectedColorInColorChooser)

    IPC_MESSAGE_HANDLER(ViewHostMsg_DidFirstVisuallyNonEmptyPaint,
                        OnFirstVisuallyNonEmptyPaint)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidLoadResourceFromMemoryCache,
                        OnDidLoadResourceFromMemoryCache)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidDisplayInsecureContent,
                        OnDidDisplayInsecureContent)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidRunInsecureContent,
                        OnDidRunInsecureContent)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidDisplayContentWithCertificateErrors,
                        OnDidDisplayContentWithCertificateErrors)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DidRunContentWithCertificateErrors,
                        OnDidRunContentWithCertificateErrors)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GoToEntryAtOffset, OnGoToEntryAtOffset)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateZoomLimits, OnUpdateZoomLimits)
    IPC_MESSAGE_HANDLER(ViewHostMsg_PageScaleFactorChanged,
                        OnPageScaleFactorChanged)
    IPC_MESSAGE_HANDLER(ViewHostMsg_EnumerateDirectory, OnEnumerateDirectory)
    IPC_MESSAGE_HANDLER(FrameHostMsg_RegisterProtocolHandler,
                        OnRegisterProtocolHandler)
    IPC_MESSAGE_HANDLER(FrameHostMsg_UnregisterProtocolHandler,
                        OnUnregisterProtocolHandler)
    IPC_MESSAGE_HANDLER(FrameHostMsg_UpdatePageImportanceSignals,
                        OnUpdatePageImportanceSignals)
    IPC_MESSAGE_HANDLER(FrameHostMsg_Find_Reply, OnFindReply)
    IPC_MESSAGE_HANDLER(ViewHostMsg_AppCacheAccessed, OnAppCacheAccessed)
    IPC_MESSAGE_HANDLER(ViewHostMsg_WebUISend, OnWebUISend)
#if defined(ENABLE_PLUGINS)
    IPC_MESSAGE_HANDLER(FrameHostMsg_PepperInstanceCreated,
                        OnPepperInstanceCreated)
    IPC_MESSAGE_HANDLER(FrameHostMsg_PepperInstanceDeleted,
                        OnPepperInstanceDeleted)
    IPC_MESSAGE_HANDLER(FrameHostMsg_PepperPluginHung, OnPepperPluginHung)
    IPC_MESSAGE_HANDLER(FrameHostMsg_PluginCrashed, OnPluginCrashed)
    IPC_MESSAGE_HANDLER(ViewHostMsg_RequestPpapiBrokerPermission,
                        OnRequestPpapiBrokerPermission)
    IPC_MESSAGE_HANDLER_GENERIC(BrowserPluginHostMsg_Attach,
                                OnBrowserPluginMessage(render_frame_host,
                                                       message))
#endif
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateFaviconURL, OnUpdateFaviconURL)
    IPC_MESSAGE_HANDLER(ViewHostMsg_ShowValidationMessage,
                        OnShowValidationMessage)
    IPC_MESSAGE_HANDLER(ViewHostMsg_HideValidationMessage,
                        OnHideValidationMessage)
    IPC_MESSAGE_HANDLER(ViewHostMsg_MoveValidationMessage,
                        OnMoveValidationMessage)
#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(FrameHostMsg_FindMatchRects_Reply,
                        OnFindMatchRectsReply)
    IPC_MESSAGE_HANDLER(ViewHostMsg_OpenDateTimeDialog,
                        OnOpenDateTimeDialog)
#endif
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  render_view_message_source_ = NULL;
  render_frame_message_source_ = NULL;

  return handled;
}
