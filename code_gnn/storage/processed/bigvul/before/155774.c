void HistoryController::RemoveChildrenForRedirect(RenderFrameImpl* frame) {
  if (!provisional_entry_)
    return;
  if (HistoryEntry::HistoryNode* node =
          provisional_entry_->GetHistoryNodeForFrame(frame))
    node->RemoveChildren();
}
