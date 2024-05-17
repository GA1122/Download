void HistoryController::UpdateForInitialLoadInChildFrame(
    RenderFrameImpl* frame,
    const WebHistoryItem& item) {
  DCHECK_NE(frame->GetWebFrame()->top(), frame->GetWebFrame());
  if (!current_entry_)
    return;
  if (HistoryEntry::HistoryNode* existing_node =
          current_entry_->GetHistoryNodeForFrame(frame)) {
    existing_node->set_item(item);
    return;
  }
  RenderFrameImpl* parent =
      RenderFrameImpl::FromWebFrame(frame->GetWebFrame()->parent());
  if (!parent)
    return;
  if (HistoryEntry::HistoryNode* parent_history_node =
          current_entry_->GetHistoryNodeForFrame(parent)) {
    parent_history_node->AddChild(item);
  }
}
