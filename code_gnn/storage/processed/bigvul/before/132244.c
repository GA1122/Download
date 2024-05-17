bool RenderFrameImpl::OnMessageReceived(const IPC::Message& msg) {
  if (!frame_)
    return false;

  if (!frame_->document().isNull())
    GetContentClient()->SetActiveURL(frame_->document().url());

  base::ObserverListBase<RenderFrameObserver>::Iterator it(&observers_);
  RenderFrameObserver* observer;
  while ((observer = it.GetNext()) != NULL) {
    if (observer->OnMessageReceived(msg))
      return true;
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RenderFrameImpl, msg)
    IPC_MESSAGE_HANDLER(FrameMsg_Navigate, OnNavigate)
    IPC_MESSAGE_HANDLER(FrameMsg_BeforeUnload, OnBeforeUnload)
    IPC_MESSAGE_HANDLER(FrameMsg_SwapOut, OnSwapOut)
    IPC_MESSAGE_HANDLER(FrameMsg_Stop, OnStop)
    IPC_MESSAGE_HANDLER(FrameMsg_ContextMenuClosed, OnContextMenuClosed)
    IPC_MESSAGE_HANDLER(FrameMsg_CustomContextMenuAction,
                        OnCustomContextMenuAction)
    IPC_MESSAGE_HANDLER(InputMsg_Undo, OnUndo)
    IPC_MESSAGE_HANDLER(InputMsg_Redo, OnRedo)
    IPC_MESSAGE_HANDLER(InputMsg_Cut, OnCut)
    IPC_MESSAGE_HANDLER(InputMsg_Copy, OnCopy)
    IPC_MESSAGE_HANDLER(InputMsg_Paste, OnPaste)
    IPC_MESSAGE_HANDLER(InputMsg_PasteAndMatchStyle, OnPasteAndMatchStyle)
    IPC_MESSAGE_HANDLER(InputMsg_Delete, OnDelete)
    IPC_MESSAGE_HANDLER(InputMsg_SelectAll, OnSelectAll)
    IPC_MESSAGE_HANDLER(InputMsg_SelectRange, OnSelectRange)
    IPC_MESSAGE_HANDLER(InputMsg_AdjustSelectionByCharacterOffset,
                        OnAdjustSelectionByCharacterOffset)
    IPC_MESSAGE_HANDLER(InputMsg_Unselect, OnUnselect)
    IPC_MESSAGE_HANDLER(InputMsg_MoveRangeSelectionExtent,
                        OnMoveRangeSelectionExtent)
    IPC_MESSAGE_HANDLER(InputMsg_Replace, OnReplace)
    IPC_MESSAGE_HANDLER(InputMsg_ReplaceMisspelling, OnReplaceMisspelling)
    IPC_MESSAGE_HANDLER(InputMsg_ExtendSelectionAndDelete,
                        OnExtendSelectionAndDelete)
    IPC_MESSAGE_HANDLER(InputMsg_SetCompositionFromExistingText,
                        OnSetCompositionFromExistingText)
    IPC_MESSAGE_HANDLER(InputMsg_ExecuteNoValueEditCommand,
                        OnExecuteNoValueEditCommand)
    IPC_MESSAGE_HANDLER(FrameMsg_CSSInsertRequest, OnCSSInsertRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_AddMessageToConsole, OnAddMessageToConsole)
    IPC_MESSAGE_HANDLER(FrameMsg_JavaScriptExecuteRequest,
                        OnJavaScriptExecuteRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_JavaScriptExecuteRequestForTests,
                        OnJavaScriptExecuteRequestForTests)
    IPC_MESSAGE_HANDLER(FrameMsg_JavaScriptExecuteRequestInIsolatedWorld,
                        OnJavaScriptExecuteRequestInIsolatedWorld)
    IPC_MESSAGE_HANDLER(FrameMsg_VisualStateRequest,
                        OnVisualStateRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_SetEditableSelectionOffsets,
                        OnSetEditableSelectionOffsets)
    IPC_MESSAGE_HANDLER(FrameMsg_Reload, OnReload)
    IPC_MESSAGE_HANDLER(FrameMsg_TextSurroundingSelectionRequest,
                        OnTextSurroundingSelectionRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_SetAccessibilityMode,
                        OnSetAccessibilityMode)
    IPC_MESSAGE_HANDLER(AccessibilityMsg_SnapshotTree,
                        OnSnapshotAccessibilityTree)
    IPC_MESSAGE_HANDLER(FrameMsg_DisownOpener, OnDisownOpener)
    IPC_MESSAGE_HANDLER(FrameMsg_CommitNavigation, OnCommitNavigation)
    IPC_MESSAGE_HANDLER(FrameMsg_DidUpdateSandboxFlags, OnDidUpdateSandboxFlags)
    IPC_MESSAGE_HANDLER(FrameMsg_SetTextTrackSettings,
                        OnTextTrackSettingsChanged)
    IPC_MESSAGE_HANDLER(FrameMsg_PostMessageEvent, OnPostMessageEvent)
    IPC_MESSAGE_HANDLER(FrameMsg_FailedNavigation, OnFailedNavigation)
#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(FrameMsg_SelectPopupMenuItems, OnSelectPopupMenuItems)
#elif defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(FrameMsg_SelectPopupMenuItem, OnSelectPopupMenuItem)
    IPC_MESSAGE_HANDLER(InputMsg_CopyToFindPboard, OnCopyToFindPboard)
#endif
  IPC_END_MESSAGE_MAP()

  return handled;
}
