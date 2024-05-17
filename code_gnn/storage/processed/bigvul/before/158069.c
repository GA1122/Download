void LocalFrameClientImpl::DispatchDidCommitLoad(
    HistoryItem* item,
    WebHistoryCommitType commit_type,
    WebGlobalObjectReusePolicy global_object_reuse_policy) {
  if (!web_frame_->Parent()) {
    web_frame_->ViewImpl()->DidCommitLoad(commit_type == kWebStandardCommit,
                                          false);
  }

  if (web_frame_->Client()) {
    web_frame_->Client()->DidCommitProvisionalLoad(
        WebHistoryItem(item), commit_type, global_object_reuse_policy);
    if (web_frame_->GetFrame()->IsLocalRoot()) {
      ResetWheelAndTouchEventHandlerProperties(*web_frame_->GetFrame());
    }
  }
  if (WebDevToolsAgentImpl* dev_tools = DevToolsAgent())
    dev_tools->DidCommitLoadForLocalFrame(web_frame_->GetFrame());

  virtual_time_pauser_.UnpauseVirtualTime();
}
