void NavigationControllerImpl::FindFramesToNavigate(
    FrameTreeNode* frame,
    FrameLoadVector* same_document_loads,
    FrameLoadVector* different_document_loads) {
  DCHECK(pending_entry_);
  DCHECK_GE(last_committed_entry_index_, 0);
  FrameNavigationEntry* new_item = pending_entry_->GetFrameEntry(frame);
  FrameNavigationEntry* old_item =
      GetLastCommittedEntry()->GetFrameEntry(frame);
  if (!new_item)
    return;

  if (!old_item ||
      new_item->item_sequence_number() != old_item->item_sequence_number() ||
      (new_item->site_instance() != nullptr &&
       new_item->site_instance() != old_item->site_instance())) {
    if (old_item &&
        new_item->document_sequence_number() ==
            old_item->document_sequence_number() &&
        !frame->current_frame_host()->GetLastCommittedURL().is_empty()) {
      same_document_loads->push_back(std::make_pair(frame, new_item));

      return;
    } else {
      different_document_loads->push_back(std::make_pair(frame, new_item));
      return;
    }
  }

  for (size_t i = 0; i < frame->child_count(); i++) {
    FindFramesToNavigate(frame->child_at(i), same_document_loads,
                         different_document_loads);
  }
}
