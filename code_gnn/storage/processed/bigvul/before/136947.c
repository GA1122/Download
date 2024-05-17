bool HTMLInputElement::ReceiveDroppedFiles(const DragData* drag_data) {
  return input_type_->ReceiveDroppedFiles(drag_data);
}
