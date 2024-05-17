void HistoryController::RecursiveGoToEntry(
    WebFrame* frame,
    HistoryFrameLoadVector& same_document_loads,
    HistoryFrameLoadVector& different_document_loads) {
  DCHECK(provisional_entry_);
  DCHECK(current_entry_);
  RenderFrameImpl* render_frame = RenderFrameImpl::FromWebFrame(frame);
  const WebHistoryItem& new_item =
      provisional_entry_->GetItemForFrame(render_frame);
  const WebHistoryItem& old_item =
      current_entry_->GetItemForFrame(render_frame);
  if (new_item.isNull())
    return;

  if (old_item.isNull() ||
      new_item.itemSequenceNumber() != old_item.itemSequenceNumber()) {
    if (!old_item.isNull() &&
        new_item.documentSequenceNumber() ==
            old_item.documentSequenceNumber()) {
      same_document_loads.push_back(std::make_pair(frame, new_item));
    } else {
      different_document_loads.push_back(std::make_pair(frame, new_item));
      return;
    }
  }

  for (WebFrame* child = frame->firstChild(); child;
       child = child->nextSibling()) {
    RecursiveGoToEntry(child, same_document_loads, different_document_loads);
  }
}
