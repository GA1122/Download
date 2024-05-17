bool RenderViewImpl::OnMessageReceived(const IPC::Message& message) {
  WebFrame* main_frame = webview() ? webview()->MainFrame() : nullptr;
  if (main_frame) {
    GURL active_url;
    if (main_frame->IsWebLocalFrame())
      active_url = main_frame->ToWebLocalFrame()->GetDocument().Url();
    GetContentClient()->SetActiveURL(
        active_url, main_frame->Top()->GetSecurityOrigin().ToString().Utf8());
  }

  if (is_swapped_out_ &&
      IPC_MESSAGE_ID_CLASS(message.type()) == InputMsgStart) {
    IPC_BEGIN_MESSAGE_MAP(RenderViewImpl, message)
      IPC_MESSAGE_HANDLER(InputMsg_HandleInputEvent, OnDiscardInputEvent)
    IPC_END_MESSAGE_MAP()
    return false;
  }

  for (auto& observer : observers_) {
    if (observer.OnMessageReceived(message))
      return true;
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RenderViewImpl, message)
    IPC_MESSAGE_HANDLER(ViewMsg_SetPageScale, OnSetPageScale)
    IPC_MESSAGE_HANDLER(ViewMsg_SetInitialFocus, OnSetInitialFocus)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateTargetURL_ACK, OnUpdateTargetURLAck)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateWebPreferences, OnUpdateWebPreferences)
    IPC_MESSAGE_HANDLER(ViewMsg_EnumerateDirectoryResponse,
                        OnEnumerateDirectoryResponse)
    IPC_MESSAGE_HANDLER(ViewMsg_ClosePage, OnClosePage)
    IPC_MESSAGE_HANDLER(ViewMsg_MoveOrResizeStarted, OnMoveOrResizeStarted)
    IPC_MESSAGE_HANDLER(ViewMsg_SetBackgroundOpaque, OnSetBackgroundOpaque)
    IPC_MESSAGE_HANDLER(ViewMsg_EnablePreferredSizeChangedMode,
                        OnEnablePreferredSizeChangedMode)
    IPC_MESSAGE_HANDLER(ViewMsg_EnableAutoResize, OnEnableAutoResize)
    IPC_MESSAGE_HANDLER(ViewMsg_DisableAutoResize, OnDisableAutoResize)
    IPC_MESSAGE_HANDLER(ViewMsg_SetLocalSurfaceIdForAutoResize,
                        OnSetLocalSurfaceIdForAutoResize)
    IPC_MESSAGE_HANDLER(ViewMsg_DisableScrollbarsForSmallWindows,
                        OnDisableScrollbarsForSmallWindows)
    IPC_MESSAGE_HANDLER(ViewMsg_SetRendererPrefs, OnSetRendererPrefs)
    IPC_MESSAGE_HANDLER(ViewMsg_MediaPlayerActionAt, OnMediaPlayerActionAt)
    IPC_MESSAGE_HANDLER(ViewMsg_PluginActionAt, OnPluginActionAt)
    IPC_MESSAGE_HANDLER(ViewMsg_SetActive, OnSetActive)
    IPC_MESSAGE_HANDLER(ViewMsg_ResolveTapDisambiguation,
                        OnResolveTapDisambiguation)
    IPC_MESSAGE_HANDLER(ViewMsg_ForceRedraw, OnForceRedraw)
    IPC_MESSAGE_HANDLER(ViewMsg_SelectWordAroundCaret, OnSelectWordAroundCaret)

    IPC_MESSAGE_HANDLER(PageMsg_UpdateWindowScreenRect,
                        OnUpdateWindowScreenRect)
    IPC_MESSAGE_HANDLER(PageMsg_SetZoomLevel, OnSetZoomLevel)
    IPC_MESSAGE_HANDLER(PageMsg_WasHidden, OnPageWasHidden)
    IPC_MESSAGE_HANDLER(PageMsg_WasShown, OnPageWasShown)
    IPC_MESSAGE_HANDLER(PageMsg_SetHistoryOffsetAndLength,
                        OnSetHistoryOffsetAndLength)
    IPC_MESSAGE_HANDLER(PageMsg_AudioStateChanged, OnAudioStateChanged)
    IPC_MESSAGE_HANDLER(PageMsg_UpdateScreenInfo, OnUpdateScreenInfo)

#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(ViewMsg_UpdateBrowserControlsState,
                        OnUpdateBrowserControlsState)
#elif defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(ViewMsg_GetRenderedText,
                        OnGetRenderedText)
    IPC_MESSAGE_HANDLER(ViewMsg_Close, OnClose)
#endif

    IPC_MESSAGE_UNHANDLED(handled = RenderWidget::OnMessageReceived(message))
  IPC_END_MESSAGE_MAP()

  return handled;
}
