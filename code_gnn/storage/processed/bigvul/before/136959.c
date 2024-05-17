void HTMLInputElement::SetCanReceiveDroppedFiles(
    bool can_receive_dropped_files) {
  if (!!can_receive_dropped_files_ == can_receive_dropped_files)
    return;
  can_receive_dropped_files_ = can_receive_dropped_files;
  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
