int TabStrip::OnDragUpdated(const DropTargetEvent& event) {
  UpdateDropIndex(event);

  if (!drop_info_->file_supported)
    return ui::DragDropTypes::DRAG_NONE;

  return GetDropEffect(event);
}
