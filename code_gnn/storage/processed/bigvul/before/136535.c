const DisplayItem* PaintController::LastDisplayItem(unsigned offset) {
  if (offset < new_display_item_list_.size())
    return &new_display_item_list_[new_display_item_list_.size() - offset - 1];
  return nullptr;
}
