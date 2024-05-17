bool RenderWidgetHostImpl::OnMessageReceived(const IPC::Message &msg) {
  if (!renderer_initialized())
    return false;

  if (owner_delegate_ && owner_delegate_->OnMessageReceived(msg))
    return true;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RenderWidgetHostImpl, msg)
    IPC_MESSAGE_HANDLER(FrameHostMsg_RenderProcessGone, OnRenderProcessGone)
    IPC_MESSAGE_HANDLER(FrameHostMsg_HittestData, OnHittestData)
    IPC_MESSAGE_HANDLER(InputHostMsg_QueueSyntheticGesture,
                        OnQueueSyntheticGesture)
    IPC_MESSAGE_HANDLER(InputHostMsg_ImeCancelComposition,
                        OnImeCancelComposition)
    IPC_MESSAGE_HANDLER(ViewHostMsg_Close, OnClose)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateScreenRects_ACK,
                        OnUpdateScreenRectsAck)
    IPC_MESSAGE_HANDLER(ViewHostMsg_RequestMove, OnRequestMove)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetTooltipText, OnSetTooltipText)
    IPC_MESSAGE_HANDLER_GENERIC(ViewHostMsg_SwapCompositorFrame,
                                OnSwapCompositorFrame(msg))
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateRect, OnUpdateRect)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetCursor, OnSetCursor)
    IPC_MESSAGE_HANDLER(ViewHostMsg_TextInputStateChanged,
                        OnTextInputStateChanged)
    IPC_MESSAGE_HANDLER(ViewHostMsg_LockMouse, OnLockMouse)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UnlockMouse, OnUnlockMouse)
    IPC_MESSAGE_HANDLER(ViewHostMsg_ShowDisambiguationPopup,
                        OnShowDisambiguationPopup)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SelectionChanged, OnSelectionChanged)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SelectionBoundsChanged,
                        OnSelectionBoundsChanged)
#if defined(OS_WIN)
    IPC_MESSAGE_HANDLER(ViewHostMsg_WindowlessPluginDummyWindowCreated,
                        OnWindowlessPluginDummyWindowCreated)
    IPC_MESSAGE_HANDLER(ViewHostMsg_WindowlessPluginDummyWindowDestroyed,
                        OnWindowlessPluginDummyWindowDestroyed)
#endif
    IPC_MESSAGE_HANDLER(InputHostMsg_ImeCompositionRangeChanged,
                        OnImeCompositionRangeChanged)
    IPC_MESSAGE_HANDLER(ViewHostMsg_DidFirstPaintAfterLoad,
                        OnFirstPaintAfterLoad)
    IPC_MESSAGE_HANDLER(ViewHostMsg_ForwardCompositorProto,
                        OnForwardCompositorProto)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (!handled && input_router_ && input_router_->OnMessageReceived(msg))
    return true;

  if (!handled && view_ && view_->OnMessageReceived(msg))
    return true;

  return handled;
}
