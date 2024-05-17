bool RenderWidgetHostImpl::OnMessageReceived(const IPC::Message &msg) {
  if (!renderer_initialized())
    return false;

  if (owner_delegate_ && owner_delegate_->OnMessageReceived(msg))
    return true;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RenderWidgetHostImpl, msg)
    IPC_MESSAGE_HANDLER(FrameHostMsg_RenderProcessGone, OnRenderProcessGone)
    IPC_MESSAGE_HANDLER(FrameHostMsg_HittestData, OnHittestData)
    IPC_MESSAGE_HANDLER(ViewHostMsg_Close, OnClose)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateScreenRects_ACK,
                        OnUpdateScreenRectsAck)
    IPC_MESSAGE_HANDLER(ViewHostMsg_RequestSetBounds, OnRequestSetBounds)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetTooltipText, OnSetTooltipText)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetCursor, OnSetCursor)
    IPC_MESSAGE_HANDLER(ViewHostMsg_AutoscrollStart, OnAutoscrollStart)
    IPC_MESSAGE_HANDLER(ViewHostMsg_AutoscrollFling, OnAutoscrollFling)
    IPC_MESSAGE_HANDLER(ViewHostMsg_AutoscrollEnd, OnAutoscrollEnd)
    IPC_MESSAGE_HANDLER(ViewHostMsg_TextInputStateChanged,
                        OnTextInputStateChanged)
    IPC_MESSAGE_HANDLER(ViewHostMsg_LockMouse, OnLockMouse)
    IPC_MESSAGE_HANDLER(ViewHostMsg_UnlockMouse, OnUnlockMouse)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SelectionBoundsChanged,
                        OnSelectionBoundsChanged)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FocusedNodeTouched, OnFocusedNodeTouched)
    IPC_MESSAGE_HANDLER(DragHostMsg_StartDragging, OnStartDragging)
    IPC_MESSAGE_HANDLER(DragHostMsg_UpdateDragCursor, OnUpdateDragCursor)
    IPC_MESSAGE_HANDLER(ViewHostMsg_ForceRedrawComplete, OnForceRedrawComplete)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FrameSwapMessages,
                        OnFrameSwapMessagesReceived)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (!handled && input_router_ && input_router_->OnMessageReceived(msg))
    return true;

  if (!handled && view_ && view_->OnMessageReceived(msg))
    return true;

  return handled;
}
