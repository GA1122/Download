bool RenderFrameImpl::OnMessageReceived(const IPC::Message& msg) {
  if ((IPC_MESSAGE_CLASS(msg) == PageMsgStart)) {
    if (render_view())
      return render_view()->OnMessageReceived(msg);

    return false;
  }

  if (!frame_)
    return false;

  DCHECK(!frame_->GetDocument().IsNull());

  GetContentClient()->SetActiveURL(
      frame_->GetDocument().Url(),
      frame_->Top()->GetSecurityOrigin().ToString().Utf8());

  {
    SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.OnMessageReceived");
    for (auto& observer : observers_) {
      if (observer.OnMessageReceived(msg))
        return true;
    }
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RenderFrameImpl, msg)
    IPC_MESSAGE_HANDLER(FrameMsg_BeforeUnload, OnBeforeUnload)
    IPC_MESSAGE_HANDLER(FrameMsg_SwapOut, OnSwapOut)
    IPC_MESSAGE_HANDLER(FrameMsg_SwapIn, OnSwapIn)
    IPC_MESSAGE_HANDLER(FrameMsg_Delete, OnDeleteFrame)
    IPC_MESSAGE_HANDLER(FrameMsg_Stop, OnStop)
    IPC_MESSAGE_HANDLER(FrameMsg_DroppedNavigation, OnDroppedNavigation)
    IPC_MESSAGE_HANDLER(FrameMsg_Collapse, OnCollapse)
    IPC_MESSAGE_HANDLER(FrameMsg_ContextMenuClosed, OnContextMenuClosed)
    IPC_MESSAGE_HANDLER(FrameMsg_CustomContextMenuAction,
                        OnCustomContextMenuAction)
#if BUILDFLAG(ENABLE_PLUGINS)
    IPC_MESSAGE_HANDLER(FrameMsg_SetPepperVolume, OnSetPepperVolume)
#endif
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
    IPC_MESSAGE_HANDLER(InputMsg_CollapseSelection, OnCollapseSelection)
    IPC_MESSAGE_HANDLER(InputMsg_MoveRangeSelectionExtent,
                        OnMoveRangeSelectionExtent)
    IPC_MESSAGE_HANDLER(InputMsg_Replace, OnReplace)
    IPC_MESSAGE_HANDLER(InputMsg_ReplaceMisspelling, OnReplaceMisspelling)
    IPC_MESSAGE_HANDLER(InputMsg_MoveCaret, OnMoveCaret)
    IPC_MESSAGE_HANDLER(InputMsg_ScrollFocusedEditableNodeIntoRect,
                        OnScrollFocusedEditableNodeIntoRect)
    IPC_MESSAGE_HANDLER(FrameMsg_CopyImageAt, OnCopyImageAt)
    IPC_MESSAGE_HANDLER(FrameMsg_SaveImageAt, OnSaveImageAt)
    IPC_MESSAGE_HANDLER(InputMsg_ExtendSelectionAndDelete,
                        OnExtendSelectionAndDelete)
    IPC_MESSAGE_HANDLER(InputMsg_DeleteSurroundingText, OnDeleteSurroundingText)
    IPC_MESSAGE_HANDLER(InputMsg_DeleteSurroundingTextInCodePoints,
                        OnDeleteSurroundingTextInCodePoints)
    IPC_MESSAGE_HANDLER(InputMsg_SetCompositionFromExistingText,
                        OnSetCompositionFromExistingText)
    IPC_MESSAGE_HANDLER(InputMsg_SetEditableSelectionOffsets,
                        OnSetEditableSelectionOffsets)
    IPC_MESSAGE_HANDLER(InputMsg_ExecuteNoValueEditCommand,
                        OnExecuteNoValueEditCommand)
    IPC_MESSAGE_HANDLER(FrameMsg_AddMessageToConsole, OnAddMessageToConsole)
    IPC_MESSAGE_HANDLER(FrameMsg_JavaScriptExecuteRequest,
                        OnJavaScriptExecuteRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_JavaScriptExecuteRequestForTests,
                        OnJavaScriptExecuteRequestForTests)
    IPC_MESSAGE_HANDLER(FrameMsg_JavaScriptExecuteRequestInIsolatedWorld,
                        OnJavaScriptExecuteRequestInIsolatedWorld)
    IPC_MESSAGE_HANDLER(FrameMsg_VisualStateRequest,
                        OnVisualStateRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_Reload, OnReload)
    IPC_MESSAGE_HANDLER(FrameMsg_ReloadLoFiImages, OnReloadLoFiImages)
    IPC_MESSAGE_HANDLER(FrameMsg_TextSurroundingSelectionRequest,
                        OnTextSurroundingSelectionRequest)
    IPC_MESSAGE_HANDLER(FrameMsg_SetAccessibilityMode,
                        OnSetAccessibilityMode)
    IPC_MESSAGE_HANDLER(AccessibilityMsg_SnapshotTree,
                        OnSnapshotAccessibilityTree)
    IPC_MESSAGE_HANDLER(FrameMsg_UpdateOpener, OnUpdateOpener)
    IPC_MESSAGE_HANDLER(FrameMsg_DidUpdateFramePolicy, OnDidUpdateFramePolicy)
    IPC_MESSAGE_HANDLER(FrameMsg_SetFrameOwnerProperties,
                        OnSetFrameOwnerProperties)
    IPC_MESSAGE_HANDLER(FrameMsg_AdvanceFocus, OnAdvanceFocus)
    IPC_MESSAGE_HANDLER(FrameMsg_AdvanceFocusInForm, OnAdvanceFocusInForm)
    IPC_MESSAGE_HANDLER(FrameMsg_SetFocusedFrame, OnSetFocusedFrame)
    IPC_MESSAGE_HANDLER(FrameMsg_SetTextTrackSettings,
                        OnTextTrackSettingsChanged)
    IPC_MESSAGE_HANDLER(FrameMsg_PostMessageEvent, OnPostMessageEvent)
    IPC_MESSAGE_HANDLER(FrameMsg_ReportContentSecurityPolicyViolation,
                        OnReportContentSecurityPolicyViolation)
    IPC_MESSAGE_HANDLER(FrameMsg_GetSavableResourceLinks,
                        OnGetSavableResourceLinks)
    IPC_MESSAGE_HANDLER(FrameMsg_GetSerializedHtmlWithLocalLinks,
                        OnGetSerializedHtmlWithLocalLinks)
    IPC_MESSAGE_HANDLER(FrameMsg_SerializeAsMHTML, OnSerializeAsMHTML)
    IPC_MESSAGE_HANDLER(FrameMsg_Find, OnFind)
    IPC_MESSAGE_HANDLER(FrameMsg_ClearActiveFindMatch, OnClearActiveFindMatch)
    IPC_MESSAGE_HANDLER(FrameMsg_StopFinding, OnStopFinding)
    IPC_MESSAGE_HANDLER(FrameMsg_EnableViewSourceMode, OnEnableViewSourceMode)
    IPC_MESSAGE_HANDLER(FrameMsg_SuppressFurtherDialogs,
                        OnSuppressFurtherDialogs)
    IPC_MESSAGE_HANDLER(FrameMsg_RunFileChooserResponse, OnFileChooserResponse)
    IPC_MESSAGE_HANDLER(FrameMsg_ClearFocusedElement, OnClearFocusedElement)
    IPC_MESSAGE_HANDLER(FrameMsg_BlinkFeatureUsageReport,
                        OnBlinkFeatureUsageReport)
    IPC_MESSAGE_HANDLER(FrameMsg_MixedContentFound, OnMixedContentFound)
    IPC_MESSAGE_HANDLER(FrameMsg_SetOverlayRoutingToken,
                        OnSetOverlayRoutingToken)
    IPC_MESSAGE_HANDLER(FrameMsg_SetHasReceivedUserGesture,
                        OnSetHasReceivedUserGesture)
#if defined(OS_ANDROID)
    IPC_MESSAGE_HANDLER(FrameMsg_ActivateNearestFindResult,
                        OnActivateNearestFindResult)
    IPC_MESSAGE_HANDLER(FrameMsg_GetNearestFindResult,
                        OnGetNearestFindResult)
    IPC_MESSAGE_HANDLER(FrameMsg_FindMatchRects, OnFindMatchRects)
#endif

#if BUILDFLAG(USE_EXTERNAL_POPUP_MENU)
#if defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(FrameMsg_SelectPopupMenuItem, OnSelectPopupMenuItem)
#else
    IPC_MESSAGE_HANDLER(FrameMsg_SelectPopupMenuItems, OnSelectPopupMenuItems)
#endif
#endif

#if defined(OS_MACOSX)
    IPC_MESSAGE_HANDLER(InputMsg_CopyToFindPboard, OnCopyToFindPboard)
#endif
  IPC_END_MESSAGE_MAP()

  return handled;
}
